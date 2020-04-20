#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/constructible_from.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_move_constructible
{
  using requirements =
      std::tuple<is_constructible_from<T, T>, is_convertible_to<T, T>>;

  static constexpr auto const value =
      is_constructible_from<T, T>::value && is_convertible_to<T, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::move_constructible");
    return 1;
  }
};

template <typename T>
constexpr auto is_move_constructible_v = is_move_constructible<T>::value;

template <typename T,
          typename = std::enable_if_t<is_move_constructible<T>::value>>
using move_constructible = T;
}
}
