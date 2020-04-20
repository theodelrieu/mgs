#pragma once

#include <tuple>
#include <type_traits>

namespace mgs
{
namespace meta
{
template <typename T, typename U>
struct is_same_as
{
  using requirements = std::tuple<>;

  static constexpr auto const value = std::is_same<T, U>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::same_as<U>");
    return 1;
  }
};

template <typename T, typename U>
constexpr auto is_same_as_v = is_same_as<T, U>::value;

template <typename T, typename U,
          typename = std::enable_if_t<is_same_as<T, U>::value>>
using same_as = T;
}
}
