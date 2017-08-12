#ifndef __COMMON_NON_COPYABLE_H__
#define __COMMON_NON_COPYABLE_H__

namespace Common
{
  
  class NonCopyable
  {
  public:
    NonCopyable(NonCopyable const &) = delete;
    NonCopyable& operator = (NonCopyable const &) = delete;
    
    NonCopyable()
    {
    }
    
  protected:
    ~NonCopyable()
    {
    }
  };
  
}

#endif  // !__COMMON_NON_COPYABLE_H__
