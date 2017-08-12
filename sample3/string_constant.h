#ifndef __NETWORK_STRING_CONSTANT_H__
#define __NETWORK_STRING_CONSTANT_H__

#define DECLARE_STRING_CONSTANT(name_, value_) \
  namespace Private \
  { \
    template <typename T> \
    struct name_ \
    { \
      static char const Name[]; \
      static char const Value[]; \
    }; \
    template <typename T> \
    char const name_ <T>::Name[] = #name_; \
    template <typename T> \
    char const name_ <T>::Value[] = #value_; \
  } \
  typedef Private:: name_ <void> name_;

#endif  // !__NETWORK_STRING_CONSTANT_H__
