#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/integral.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_signed_integral
{
  using requirements = std::tuple<meta::is_integral<T>>;

  static constexpr auto const value =
      meta::is_integral<T>::value && std::is_signed<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::signed_integral");
    return 1;
  }
};

template <typename T>
constexpr auto is_signed_integral_v = is_signed_integral<T>::value;

template <typename T,
          typename = std::enable_if_t<is_signed_integral<T>::value>>
using signed_integral = T;
}
}
