#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_regular
{
  static constexpr auto const value =
      is_semiregular<T>::value && is_equality_comparable<T>::value;

  using requirements = std::tuple<is_semiregular<T>, is_equality_comparable<T>>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::regular");
    return 1;
  }
};

template <typename T>
constexpr auto is_regular_v = is_regular<T>::value;

template <typename T, typename = std::enable_if_t<is_regular<T>::value>>
using regular = T;
}
}
