#ifndef __NETWORK_HTTP_CONTENT_TYPE_H__
#define __NETWORK_HTTP_CONTENT_TYPE_H__

#include "string_constant.h"
#include "static_counter.h"
#include "type_registry.h"

#include <cstring>
#include <stdexcept>
#include <string>

#define DECLARE_HTTP_CONTENT_TYPE(name_, value_) \
  DECLARE_STRING_CONSTANT(name_, value_) \
  namespace Private \
  { \
    GET_NEXT_STATIC_COUNTER(Counter, Id__##name_) \
    REGISTRY_ADD_TYPE(Registry, Counter::Id__##name_, \
      Network::Http::Content::Type::name_) \
  }

namespace Network
{
  namespace Http
  {
    namespace Content
    {
      namespace Type
      {
        namespace Private
        {
          INIT_STATIC_COUNTER(Counter, 200)
          DECLARE_TYPE_REGISTRY(Registry)
        }
        
        DECLARE_HTTP_CONTENT_TYPE(bin, application/octet-stream)
        DECLARE_HTTP_CONTENT_TYPE(oda, application/oda)
        DECLARE_HTTP_CONTENT_TYPE(pdf, application/pdf)
        DECLARE_HTTP_CONTENT_TYPE(ai, application/postscript)
        DECLARE_HTTP_CONTENT_TYPE(eps, application/postscript)
        DECLARE_HTTP_CONTENT_TYPE(ps, application/postscript)
        DECLARE_HTTP_CONTENT_TYPE(rtf, application/rtf)
        DECLARE_HTTP_CONTENT_TYPE(mif, application/x-mif)
        DECLARE_HTTP_CONTENT_TYPE(csh, application/x-csh)
        DECLARE_HTTP_CONTENT_TYPE(dvi, application/x-dvi)
        DECLARE_HTTP_CONTENT_TYPE(hdf, application/x-hdf)
        DECLARE_HTTP_CONTENT_TYPE(latex, application/x-latex)
        DECLARE_HTTP_CONTENT_TYPE(nc, application/x-netcdf)
        DECLARE_HTTP_CONTENT_TYPE(cdf, application/x-netcdf)
        DECLARE_HTTP_CONTENT_TYPE(sh, application/x-sh)
        DECLARE_HTTP_CONTENT_TYPE(tcl, application/x-tcl)
        DECLARE_HTTP_CONTENT_TYPE(tex, application/x-tex)
        DECLARE_HTTP_CONTENT_TYPE(texinfo, application/x-texinfo)
        DECLARE_HTTP_CONTENT_TYPE(texi, application/x-texinfo)
        DECLARE_HTTP_CONTENT_TYPE(roff, application/x-troff)
        DECLARE_HTTP_CONTENT_TYPE(man, application/x-troff-man)
        DECLARE_HTTP_CONTENT_TYPE(me, application/x-troff-me)
        DECLARE_HTTP_CONTENT_TYPE(ms, application/x-troff-ms)
        DECLARE_HTTP_CONTENT_TYPE(src, application/x-wais-source)
        DECLARE_HTTP_CONTENT_TYPE(zip, application/zip)
        DECLARE_HTTP_CONTENT_TYPE(bcpio, application/x-bcpio)
        DECLARE_HTTP_CONTENT_TYPE(cpio, application/x-cpio)
        DECLARE_HTTP_CONTENT_TYPE(gtar, application/x-gtar)
        DECLARE_HTTP_CONTENT_TYPE(shar, application/x-shar)
        DECLARE_HTTP_CONTENT_TYPE(sv4cpio, application/x-sv4cpio)
        DECLARE_HTTP_CONTENT_TYPE(sv4crc, application/x-sv4crc)
        DECLARE_HTTP_CONTENT_TYPE(tar, application/x-tar)
        DECLARE_HTTP_CONTENT_TYPE(ustar, application/x-ustar)
        DECLARE_HTTP_CONTENT_TYPE(au, audio/basic)
        DECLARE_HTTP_CONTENT_TYPE(snd, audio/basic)
        DECLARE_HTTP_CONTENT_TYPE(aif, audio/x-aiff)
        DECLARE_HTTP_CONTENT_TYPE(aiff, audio/x-aiff)
        DECLARE_HTTP_CONTENT_TYPE(aifc, audio/x-aiff)
        DECLARE_HTTP_CONTENT_TYPE(wav, audio/x-wav)
        DECLARE_HTTP_CONTENT_TYPE(gif, image/gif)
        DECLARE_HTTP_CONTENT_TYPE(ief, image/ief)
        DECLARE_HTTP_CONTENT_TYPE(jpeg, image/jpeg)
        DECLARE_HTTP_CONTENT_TYPE(jpg, image/jpeg)
        DECLARE_HTTP_CONTENT_TYPE(jpe, image/jpeg)
        DECLARE_HTTP_CONTENT_TYPE(tiff, image/tiff)
        DECLARE_HTTP_CONTENT_TYPE(tif, image/tiff)
        DECLARE_HTTP_CONTENT_TYPE(ras, image/x-cmu-raster)
        DECLARE_HTTP_CONTENT_TYPE(pnm, image/x-portable-anymap)
        DECLARE_HTTP_CONTENT_TYPE(pbm, image/x-portable-bitmap)
        DECLARE_HTTP_CONTENT_TYPE(pgm, image/x-portable-graymap)
        DECLARE_HTTP_CONTENT_TYPE(ppm, image/x-portable-pixmap)
        DECLARE_HTTP_CONTENT_TYPE(png, image/png)
        DECLARE_HTTP_CONTENT_TYPE(rgb, image/x-rgb)
        DECLARE_HTTP_CONTENT_TYPE(xbm, image/x-xbitmap)
        DECLARE_HTTP_CONTENT_TYPE(xpm, image/x-xpixmap)
        DECLARE_HTTP_CONTENT_TYPE(xwd, image/x-xwindowdump)
        DECLARE_HTTP_CONTENT_TYPE(html, text/html)
        DECLARE_HTTP_CONTENT_TYPE(txt, text/plain)
        DECLARE_HTTP_CONTENT_TYPE(rtx, text/richtext)
        DECLARE_HTTP_CONTENT_TYPE(tsv, text/tab-separated-values)
        DECLARE_HTTP_CONTENT_TYPE(ext, text/x-setext)
        DECLARE_HTTP_CONTENT_TYPE(mpeg, video/mpeg)
        DECLARE_HTTP_CONTENT_TYPE(mpg, video/mpeg)
        DECLARE_HTTP_CONTENT_TYPE(mpe, video/mpeg)
        DECLARE_HTTP_CONTENT_TYPE(qt, video/quicktime)
        DECLARE_HTTP_CONTENT_TYPE(mov, video/quicktime)
        DECLARE_HTTP_CONTENT_TYPE(avi, video/x-msvideo)
        DECLARE_HTTP_CONTENT_TYPE(movie, video/x-sgi-movie)
        

        namespace Private
        {
          GET_NEXT_STATIC_COUNTER(Counter, LastTypeCounter)
              
          template <unsigned N>
          struct FindType
          {
            static char const* Find(char const *ext)
            {
              if (!std::strcmp(Registry<N>::Type::Name, ext))
                return Registry<N>::Type::Value;
              return FindType<N - 1>::Find(ext);
            }
          };
          
          template <>
          struct FindType<0>
          {
            static char const* Find(char const *)
            {
              return "";
            }
          };
          
          inline char const* FileExtToType(char const *ext)
          {
            return FindType<Counter::LastTypeCounter - 1>::Find(ext);
          }
          
        }
      }
      
      inline char const* FileExtToType(char const *ext)
      {
        return Type::Private::FileExtToType(ext);
      }
      
      inline char const* TypeFromFileName(std::string const &fileName)
      {
        auto Pos = fileName.rfind('.');
        if (Pos == std::string::npos)
          return "";
        return FileExtToType(fileName.substr(Pos + 1).c_str());
      }

    }
  }
}

#endif  // !__NETWORK_HTTP_CONTENT_TYPE_H__
