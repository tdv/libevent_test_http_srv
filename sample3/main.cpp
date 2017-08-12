#include "http_server.h"
#include "http_headers.h"
#include "http_content_type.h"

#include <iostream>
#include <sstream>
#include <mutex>

int main()
{
  char const SrvAddress[] = "127.0.0.1";
  std::uint16_t SrvPort = 5555;
  std::uint16_t SrvThreadCount = 4;
  std::string const RootDir = "../test_content";
  std::string const DefaultPage = "index.html";
  std::mutex Mtx;
  try
  {
    using namespace Network;
    HttpServer Srv(SrvAddress, SrvPort, SrvThreadCount,
      [&] (IHttpRequestPtr req)
      {
        std::string Path = req->GetPath();
        Path = RootDir + Path + (Path == "/" ? DefaultPage : std::string());
        {
          std::stringstream Io;
          Io << "Path: " << Path << std::endl
             << Http::Request::Header::Host::Name << ": "
                  << req->GetHeaderAttr(Http::Request::Header::Host::Value) << std::endl
             << Http::Request::Header::Referer::Name << ": "
                  << req->GetHeaderAttr(Http::Request::Header::Referer::Value) << std::endl;
          std::lock_guard<std::mutex> Lock(Mtx);
          std::cout << Io.str() << std::endl;
        }
        req->SetResponseAttr(Http::Response::Header::Server::Value, "MyTestServer");
        req->SetResponseAttr(Http::Response::Header::ContentType::Value,
                             Http::Content::TypeFromFileName(Path));
        req->SetResponseFile(Path);
      });
    std::cin.get();
  }
  catch (std::exception const &e)
  {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
