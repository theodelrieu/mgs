
#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/assignable_from.hpp>
#include <mgs/meta/concepts/move_constructible.hpp>
#include <mgs/meta/concepts/swappable.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_movable
{
  using requirements =
      std::tuple<is_assignable_from<std::add_lvalue_reference_t<T>, T>,
                 is_move_constructible<T>,
                 is_swappable<T>>;

  static constexpr auto const value =
      std::is_object<T>::value &&
      is_assignable_from<std::add_lvalue_reference_t<T>, T>::value &&
      is_move_constructible<T>::value && is_swappable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::movable");
    static_assert(std::is_object<T>::value, "T must be an object type");
    return 1;
  }
};

template <typename T>
constexpr auto is_movable_v = is_movable<T>::value;

template <typename T, typename = std::enable_if_t<is_movable<T>::value>>
using movable = T;
}
}
