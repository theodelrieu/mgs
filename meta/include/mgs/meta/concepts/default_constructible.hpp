#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/constructible_from.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/concepts/move_constructible.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_default_constructible
{
  using requirements = std::tuple<>;

  static constexpr auto const value = is_constructible_from<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::default_constructible");
    return 1;
  }
};

template <typename T>
constexpr auto is_default_constructible_v = is_default_constructible<T>::value;

template <typename T,
          typename = std::enable_if_t<is_default_constructible<T>::value>>
using default_constructible = T;
}
}
