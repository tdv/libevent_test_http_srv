#ifndef __COMMON_TOOLS_H__
#define __COMMON_TOOLS_H__

#include "non_copyable.h"

namespace Common
{
  
  class BoolFlagInvertor final
    : private NonCopyable
  {
  public:
    BoolFlagInvertor(bool volatile *flag)
      : Flag(flag)
    {
    }
    ~BoolFlagInvertor()
    {
      *Flag = !*Flag;
    }
    
  private:
    bool volatile *Flag;
  };
  
}

#endif  // !__COMMON_TOOLS_H__
