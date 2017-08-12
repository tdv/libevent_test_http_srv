#ifndef __NETWORK_HTTP_SERVER_H__
#define __NETWORK_HTTP_SERVER_H__

#include "non_copyable.h"
#include "exceptions.h"
#include "tools.h"
#include "http_request.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <string>

namespace Network
{
  
  DECLARE_RUNTIME_EXCEPTION(HttpServer)

  class HttpServer final
    : private Common::NonCopyable
  {
  public:
    typedef std::vector<IHttpRequest::Type> MethodPool;
    typedef std::function<void (IHttpRequestPtr)> OnRequestFunc;
    enum { MaxHeaderSize = static_cast<std::size_t>(-1), MaxBodySize = MaxHeaderSize };
    HttpServer(std::string const &address, std::uint16_t port,
               std::uint16_t threadCount, OnRequestFunc const &onRequest,
               MethodPool const &allowedMethods = {IHttpRequest::Type::GET },
               std::size_t maxHeadersSize = MaxHeaderSize,
               std::size_t maxBodySize = MaxBodySize);

  private:
    volatile bool IsRun = true;
    void (*ThreadDeleter)(std::thread *t) = [] (std::thread *t) { t->join(); delete t; };;
    typedef std::unique_ptr<std::thread, decltype(ThreadDeleter)> ThreadPtr;
    typedef std::vector<ThreadPtr> ThreadPool;
    ThreadPool Threads;
    Common::BoolFlagInvertor RunFlag;
  };
  
}


#endif  // !__NETWORK_HTTP_SERVER_H__
