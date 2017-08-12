#include "http_server.h"

#include <chrono>
#include <sstream>
#include <cstring>

#include <evhttp.h>
#include <fcntl.h>

#ifdef _WIN32
  #include <io.h>
  #define open _open
  #define lseek _lseek
  #define close _close
#else
  #include <unistd.h>
  #define open open
#define lseek lseek
#define close close
#endif

namespace Network
{
  
  namespace
  {
    class HttpRequest final
      : private Common::NonCopyable
      , public IHttpRequest
    {
    public:
      HttpRequest(evhttp_request *request)
        : Request(request)
      {
      }
      int GetResponseCode() const
      {
        return ResponseCode;
      }
      
    private:
      evhttp_request *Request;
      evkeyvalq *InputHeaders = nullptr;
      evkeyvalq *OutputHeaders = nullptr;
      evbuffer *InputBuf = nullptr;
      evbuffer *OutputBuf = nullptr;
      evhttp_uri const *Uri = nullptr;
      int ResponseCode = HTTP_OK;
      
      void GetUri() const
      {
        if (Uri)
          return;
        auto *This = const_cast<HttpRequest *>(this);
        if (!(This->Uri = evhttp_request_get_evhttp_uri(Request)))
          throw HttpRequestException("Failed to get uri.");
      }
      void GetOutputBuf() const
      {
        if (OutputBuf)
          return;
        auto *This = const_cast<HttpRequest *>(this);
        if (!(This->OutputBuf = evhttp_request_get_output_buffer(Request)))
          throw HttpRequestException("Failed to get output buffer.");
      }
      void GetInputHeaders() const
      {
        if (InputHeaders != nullptr)
          return;
        auto *This = const_cast<HttpRequest *>(this);
        if (!(This->InputHeaders = evhttp_request_get_input_headers(Request)))
          throw HttpRequestException("Failed to get http input headers.");
      }
      void GetOutputHeaders() const
      {
        if (OutputHeaders != nullptr)
          return;
        auto *This = const_cast<HttpRequest *>(this);
        if (!(This->OutputHeaders = evhttp_request_get_output_headers(Request)))
          throw HttpRequestException("Failed to get http output headers.");
      }
      static void FreeBuf(void const *data, std::size_t datalen, void *extra)
      {
        ::operator delete (const_cast<void *>(data));
      }
      
      virtual Type GetRequestType() const
      {
        switch (evhttp_request_get_command(Request))
        {
        case EVHTTP_REQ_GET :
          return Type::GET;
        case EVHTTP_REQ_POST :
          return Type::POST;
        case EVHTTP_REQ_HEAD :
          return Type::HEAD;
        case  EVHTTP_REQ_PUT :
          return Type::PUT;
        default :
          break;
        }
        throw HttpRequestException("Unknown request type.");
      }
      virtual std::string const GetHeaderAttr(char const *attrName) const
      {
        GetInputHeaders();
        char const *Ret = evhttp_find_header(InputHeaders, attrName);
        return Ret ? Ret : "";
      }
      virtual std::size_t GetContentSize() const
      {
        if (InputBuf == nullptr)
        {
          auto *This = const_cast<HttpRequest *>(this);
          if (!(This->InputBuf = evhttp_request_get_input_buffer(Request)))
            throw HttpRequestException("Failed to get input buffer.");
        }
        return evbuffer_get_length(InputBuf);
      }
      virtual void GetContent(void *buf, std::size_t len, bool remove) const
      {
        if (len > GetContentSize())
          throw HttpRequestException("Required length of data buffer more than exists.");
        if (remove)
        {
          if (evbuffer_remove(InputBuf, buf, len) == -1)
            throw HttpRequestException("Failed to get input data.");
          return;
        }
        if (evbuffer_copyout(InputBuf, buf, len) == -1)
          throw HttpRequestException("Failed to get input data.");
      }
      virtual std::string const GetPath() const
      {
        GetUri();
        char const *Ret = evhttp_uri_get_path(Uri);
        return Ret ? Ret : "";
      }
      virtual RequestParams const GetParams() const
      {
        GetUri();
        RequestParams Params;
        char const *Query = evhttp_uri_get_query(Uri);
        if (!Query)
          return std::move(Params);
        std::stringstream Io;
        Io << Query;
        for (std::string s ; Io ; )
        {
          std::getline(Io, s, '&');
          auto Pos = s.find('=');
          if (Pos != std::string::npos)
            Params[s.substr(0, Pos)] = s.substr(Pos + 1);
          else
            Params[s] = "";
        }
        return std::move(Params);
      }
      virtual void SetResponseAttr(std::string const &name, std::string const &val)
      {
        GetOutputHeaders();
        if (evhttp_add_header(OutputHeaders, name.c_str(), val.c_str()) == -1)
          throw HttpRequestException("Failed to set response header attribute.");
      }
      virtual void SetResponseCode(int code)
      {
        ResponseCode = code;
      }
      virtual void SetResponseString(std::string const &str)
      {
        GetOutputBuf();
        if (evbuffer_add_printf(OutputBuf, str.c_str()) == -1)
          throw HttpRequestException("Failed to make response.");
      }
      virtual void SetResponseBuf(void const *data, std::size_t bytes)
      {
        GetOutputBuf();
        void *Data = ::operator new (bytes);
        std::memcpy(Data, data, bytes);
        if (evbuffer_add_reference(OutputBuf, Data, bytes, &HttpRequest::FreeBuf, nullptr) == -1)
        {
          ::operator delete (Data);
          throw HttpRequestException("Failed to make response.");
        }
      }
      virtual void SetResponseFile(std::string const &fileName)
      {
        GetOutputBuf();
        auto FileDeleter = [] (int *f) { if (*f != -1) close(*f); delete f; };
        std::unique_ptr<int, decltype(FileDeleter)> File(new int(open(fileName.c_str(), 0)), FileDeleter);
        if (*File == -1)
          throw HttpRequestException(HTTP_NOTFOUND, "Could not find content for uri.");
        ev_off_t Length = lseek(*File, 0, SEEK_END);
        if (Length == -1 || lseek(*File, 0, SEEK_SET) == -1)
          throw HttpRequestException("Failed to calc file size.");
        if (evbuffer_add_file(OutputBuf, *File, 0, Length) == -1)
          throw HttpRequestException("Failed to make response.");
        *File.get() = -1;
      }
    };
    
    struct RequestParams
    {
      HttpServer::OnRequestFunc Func;
      bool volatile *Process = nullptr;
    };
    
    void OnRawRequest(evhttp_request *request, void *prm)
    {
      try
      {
        auto Request = std::make_shared<HttpRequest>(request);
        auto *ReqPrm = reinterpret_cast<RequestParams *>(prm);
        Common::BoolFlagInvertor FlagInvertor(ReqPrm->Process);
        ReqPrm->Func(Request);
        
        auto *OutBuf = evhttp_request_get_output_buffer(request);
        if (!OutBuf)
          throw HttpRequestException("Failed to get output buffer.");
        evhttp_send_reply(request, Request->GetResponseCode(), "", OutBuf);
      }
      catch (HttpRequestException const &e)
      {
        if (e.GetCode())
        {
          std::stringstream Io;
          Io << "<html><body>"
                "<hr/><center><h1>"
                << e.GetCode()
                << ". "
                << e.what()
                << "</center><hr/></h1>"
                << "</body></html>";
          evhttp_send_error(request, e.GetCode(), Io.str().c_str());
        }
        else
        {
          evhttp_send_error(request, HTTP_INTERNAL,
            "<html><body>"
            "<hr/><center><h1>500. Internal error.</center><hr/></h1>"
            "</body></html>");
        }
      }
      catch (std::exception const &e)
      {
        evhttp_send_error(request, HTTP_INTERNAL,
          "<html><body>"
          "<hr/><center><h1>500. Internal error.</center><hr/></h1>"
          "</body></html>");
      }
    }
    
    int HttpRequestTypeToAllowedMethod(IHttpRequest::Type const &type)
    {
      switch (type)
      {
      case IHttpRequest::Type::GET :
        return EVHTTP_REQ_GET;
      case IHttpRequest::Type::HEAD :
        return EVHTTP_REQ_HEAD;
      case IHttpRequest::Type::PUT :
        return EVHTTP_REQ_PUT;
      case IHttpRequest::Type::POST :
        return EVHTTP_REQ_POST;
      default :
          break;
      }
      throw HttpRequestException("Method not allowed.");
    }

  }

  HttpServer::HttpServer(std::string const &address, std::uint16_t port,
              std::uint16_t threadCount, OnRequestFunc const &onRequest,
              MethodPool const &allowedMethods,
              std::size_t maxHeadersSize, std::size_t maxBodySize)
    : RunFlag(&IsRun)
  {
    int AllowedMethods = -1;
    for (auto const i : allowedMethods)
      AllowedMethods |= HttpRequestTypeToAllowedMethod(i);
    bool volatile DoneInitThread = false;
    std::exception_ptr Except;
    evutil_socket_t Socket = -1;
    auto ThreadFunc = [&] ()
    {
      try
      {
        bool volatile ProcessRequest = false;
        RequestParams ReqPrm;
        ReqPrm.Func = onRequest;
        ReqPrm.Process = &ProcessRequest;
        typedef std::unique_ptr<event_base, decltype(&event_base_free)> EventBasePtr;
        EventBasePtr EventBase(event_base_new(), &event_base_free);
        if (!EventBase)
          throw HttpServerException("Failed to create new base_event.");
        typedef std::unique_ptr<evhttp, decltype(&evhttp_free)> EvHttpPtr;
        EvHttpPtr EvHttp(evhttp_new(EventBase.get()), &evhttp_free);
        if (!EvHttp)
          throw HttpServerException("Failed to create new evhttp.");
        evhttp_set_allowed_methods(EvHttp.get(), AllowedMethods);
        if (maxHeadersSize != MaxHeaderSize)
          evhttp_set_max_headers_size(EvHttp.get(), maxHeadersSize);
        if (maxBodySize != MaxBodySize)
          evhttp_set_max_body_size(EvHttp.get(), maxBodySize);
        evhttp_set_gencb(EvHttp.get(), &OnRawRequest, &ReqPrm);
        if (Socket == -1)
        {
          auto *BoundSock = evhttp_bind_socket_with_handle(EvHttp.get(), address.c_str(), port);
          if (!BoundSock)
            throw HttpServerException("Failed to bind server socket.");
          if ((Socket = evhttp_bound_socket_get_fd(BoundSock)) == -1)
            throw HttpServerException("Failed to get server socket for next instance.");
        }
        else
        {
          if (evhttp_accept_socket(EvHttp.get(), Socket) == -1)
            throw HttpServerException("Failed to bind server socket for new instance.");
        }
        DoneInitThread = true;
        for ( ; IsRun ; )
        {
          ProcessRequest = false;
          if (event_base_loop(EventBase.get(), EVLOOP_NONBLOCK) == -1)
          {
            // TODO: to log
            //std::cerr << "Loop error." << std::endl;
          }
          if (!ProcessRequest)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
      }
      catch (...)
      {
        Except = std::current_exception();
      }
    };
    ThreadPool NewThreads;
    for (int i = 0 ; i < threadCount ; ++i)
    {
      DoneInitThread = false;
      ThreadPtr Thread(new std::thread(ThreadFunc), ThreadDeleter);
      NewThreads.push_back(std::move(Thread));
      for ( ; ; )
      {
        if (Except != std::exception_ptr())
        {
          IsRun = false;
          std::rethrow_exception(Except);
        }
        if (DoneInitThread)
          break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
    Threads = std::move(NewThreads);
  }

}
    
