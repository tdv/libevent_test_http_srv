#ifndef __NETWORK_HTTP_REQUEST_H__
#define __NETWORK_HTTP_REQUEST_H__

#include "exceptions.h"

#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(HttpRequest)

  struct IHttpRequest
  {
    enum class Type
    {
      HEAD, GET, PUT, POST
    };
    
    typedef std::unordered_map<std::string, std::string> RequestParams;
    
    virtual ~IHttpRequest() {}
    
    virtual Type GetRequestType() const = 0;
    virtual std::string const GetHeaderAttr(char const *attrName) const = 0;
    
    virtual std::size_t GetContentSize() const = 0;
    virtual void GetContent(void *buf, std::size_t len, bool remove) const = 0;
    
    virtual std::string const GetPath() const = 0;
    virtual RequestParams const GetParams() const = 0;
    
    virtual void SetResponseAttr(std::string const &name, std::string const &val) = 0;
    virtual void SetResponseCode(int code) = 0;
    
    virtual void SetResponseString(std::string const &str) = 0;
    virtual void SetResponseBuf(void const *data, std::size_t bytes) = 0;
    virtual void SetResponseFile(std::string const &fileName) = 0;
  };
  
  typedef std::shared_ptr<IHttpRequest> IHttpRequestPtr;
  
}

#endif  // !__NETWORK_HTTP_REQUEST_H__
