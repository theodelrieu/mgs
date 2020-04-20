#pragma once

#include <utility>

#define MGS_META_DETECTED_MEMBER_FUNCTION(fun) \
  template <typename T, typename... Args>      \
  using fun = decltype(std::declval<T>().fun(std::declval<Args>()...))

#define MGS_META_DETECTED_STATIC_MEMBER_FUNCTION(fun) \
  template <typename T, typename... Args>      \
  using fun = decltype(T::fun(std::declval<Args>()...))

#define MGS_META_DETECTED_TYPE(fun) \
  template <typename T>             \
  using fun = typename T::fun
