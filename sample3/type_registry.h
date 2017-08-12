#ifndef __COMMON_TYPE_REGISTRY_H__
#define __COMMON_TYPE_REGISTRY_H__

#define DECLARE_TYPE_REGISTRY(reg_name_) \
  template <unsigned> \
  struct reg_name_;

#define REGISTRY_ADD_TYPE(reg_name_, type_id_, type_) \
  template <> \
  struct reg_name_ <type_id_> \
  { \
    typedef type_ Type; \
  };

#endif  // !__COMMON_TYPE_REGISTRY_H__
