#pragma once

#include <tuple>
#include <type_traits>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_integral
{
  using requirements = std::tuple<>;

  static constexpr auto const value = std::is_integral<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::integral");
    return 1;
  }
};

template <typename T>
constexpr auto is_integral_v = is_integral<T>::value;

template <typename T,
          typename = std::enable_if_t<is_integral<T>::value>>
using integral = T;
}
}
