#ifndef __COMMON_STATIC_COUNTER_H__
#define __COMMON_STATIC_COUNTER_H__

namespace Common
{
  
  namespace Private
  {
    
    template <unsigned N>
    struct Hierarchy
      : public Hierarchy<N - 1>
    {
    };
    
    template <>
    struct Hierarchy<0>
    {
    };
    
  }
  
}

#define INIT_STATIC_COUNTER(counter_name_, max_count_) \
  namespace counter_name_ \
  { \
    typedef ::Common::Private::Hierarchy<max_count_> CounterHierarchyType; \
    char (&GetCounterValue(void const *))[1]; \
  }

#define GET_NEXT_STATIC_COUNTER(counter_name_, value_name_) \
  namespace counter_name_ \
  { \
    enum { value_name_ = sizeof(GetCounterValue(static_cast<CounterHierarchyType const *>(0))) }; \
    char (&GetCounterValue(::Common::Private::Hierarchy<value_name_> const *))[value_name_ + 1]; \
  }

#endif  // !__COMMON_STATIC_COUNTER_H__
