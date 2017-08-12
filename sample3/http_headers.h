#ifndef __NETWORK_HTTP_HEADERS_H__
#define __NETWORK_HTTP_HEADERS_H__

#include "string_constant.h"

namespace Network
{
  namespace Http
  {
    namespace Request
    {
      namespace Header
      {
        DECLARE_STRING_CONSTANT(Accept, Accept)
        DECLARE_STRING_CONSTANT(AcceptCharset, Accept-Charset)
        DECLARE_STRING_CONSTANT(AcceptEncoding, Accept-Encoding)
        DECLARE_STRING_CONSTANT(AcceptLanguage, Accept-Language)
        DECLARE_STRING_CONSTANT(AcceptDatetime, Accept-Datetime)
        DECLARE_STRING_CONSTANT(Authorization, Authorization)
        DECLARE_STRING_CONSTANT(CacheControl, Cache-Control)
        DECLARE_STRING_CONSTANT(Connection, Connection)
        DECLARE_STRING_CONSTANT(Cookie, Cookie)
        DECLARE_STRING_CONSTANT(ContentLength, Content-Length)
        DECLARE_STRING_CONSTANT(ContentMD5, Content-MD5)
        DECLARE_STRING_CONSTANT(ContentType, Content-Type)
        DECLARE_STRING_CONSTANT(Date, Date)
        DECLARE_STRING_CONSTANT(Expect, Expect)
        DECLARE_STRING_CONSTANT(From, From)
        DECLARE_STRING_CONSTANT(Host, Host)
        DECLARE_STRING_CONSTANT(IfMatch, If-Match)
        DECLARE_STRING_CONSTANT(IfModifiedSince, If-Modified-Since)
        DECLARE_STRING_CONSTANT(IfNoneMatch, If-None-Match)
        DECLARE_STRING_CONSTANT(IfRange, If-Range)
        DECLARE_STRING_CONSTANT(IfUnmodifiedSince, If-Unmodified-Since)
        DECLARE_STRING_CONSTANT(MaxForwards, Max-Forwards)
        DECLARE_STRING_CONSTANT(Origin, Origin)
        DECLARE_STRING_CONSTANT(Pragma, Pragma)
        DECLARE_STRING_CONSTANT(ProxyAuthorization, Proxy-Authorization)
        DECLARE_STRING_CONSTANT(Range, Range)
        DECLARE_STRING_CONSTANT(Referer, Referer)
        DECLARE_STRING_CONSTANT(TE, TE)
        DECLARE_STRING_CONSTANT(UserAgent, User-Agent)
        DECLARE_STRING_CONSTANT(Via, Via)
        DECLARE_STRING_CONSTANT(Warning, Warning)
      }
      
    }
    
    namespace Response
    {
      
      namespace Header
      {
        DECLARE_STRING_CONSTANT(AccessControlAllowOrigin, Access-Control-Allow-Origin)
        DECLARE_STRING_CONSTANT(AcceptRanges, Accept-Ranges)
        DECLARE_STRING_CONSTANT(Age, Age)
        DECLARE_STRING_CONSTANT(Allow, Allow)
        DECLARE_STRING_CONSTANT(CacheControl, Cache-Control)
        DECLARE_STRING_CONSTANT(Connection, Connection)
        DECLARE_STRING_CONSTANT(ContentEncoding, Content-Encoding)
        DECLARE_STRING_CONSTANT(ContentLanguage, Content-Language)
        DECLARE_STRING_CONSTANT(ContentLength, Content-Length)
        DECLARE_STRING_CONSTANT(ContentLocation, Content-Location)
        DECLARE_STRING_CONSTANT(ContentMD5, Content-MD5)
        DECLARE_STRING_CONSTANT(ContentRange, Content-Range)
        DECLARE_STRING_CONSTANT(ContentType, Content-Type)
        DECLARE_STRING_CONSTANT(Date, Date)
        DECLARE_STRING_CONSTANT(ETag, ETag)
        DECLARE_STRING_CONSTANT(Expires, Expires)
        DECLARE_STRING_CONSTANT(LastModified, Last-Modified)
        DECLARE_STRING_CONSTANT(Link, Link)
        DECLARE_STRING_CONSTANT(Location, Location)
        DECLARE_STRING_CONSTANT(P3P, P3P)
        DECLARE_STRING_CONSTANT(Pragma, Pragma)
        DECLARE_STRING_CONSTANT(ProxyAuthenticate, Proxy-Authenticate)
        DECLARE_STRING_CONSTANT(Refresh, Refresh)
        DECLARE_STRING_CONSTANT(RetryAfter, Retry-After)
        DECLARE_STRING_CONSTANT(Server, Server)
        DECLARE_STRING_CONSTANT(SetCookie, Set-Cookie)
        DECLARE_STRING_CONSTANT(Status, Status)
        DECLARE_STRING_CONSTANT(StrictTransportSecurity, Strict-Transport-Security)
        DECLARE_STRING_CONSTANT(Trailer, Trailer)
        DECLARE_STRING_CONSTANT(TransferEncoding, Transfer-Encoding)
        DECLARE_STRING_CONSTANT(Upgrade, Upgrade)
        DECLARE_STRING_CONSTANT(Vary, Vary)
        DECLARE_STRING_CONSTANT(Via, Via)
        DECLARE_STRING_CONSTANT(Warning, Warning)
        DECLARE_STRING_CONSTANT(WWWAuthenticate, WWW-Authenticate)
      }
    }
  }
}

#endif  // !__NETWORK_HTTP_HEADERS_H__
