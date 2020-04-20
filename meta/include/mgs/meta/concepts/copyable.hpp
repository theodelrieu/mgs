
#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/assignable_from.hpp>
#include <mgs/meta/concepts/copy_constructible.hpp>
#include <mgs/meta/concepts/movable.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_copyable
{
  using requirements = std::tuple<
      is_assignable_from<std::add_lvalue_reference_t<T>,
                         std::add_lvalue_reference_t<std::add_const_t<T>>>,
      is_movable<T>,
      is_copy_constructible<T>>;

  static constexpr auto const value =
      is_assignable_from<
          std::add_lvalue_reference_t<T>,
          std::add_lvalue_reference_t<std::add_const_t<T>>>::value &&
      is_movable<T>::value && is_copy_constructible<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::copyable");
    return 1;
  }
};

template <typename T>
constexpr auto is_copyable_v = is_copyable<T>::value;

template <typename T, typename = std::enable_if_t<is_copyable<T>::value>>
using copyable = T;
}
}
