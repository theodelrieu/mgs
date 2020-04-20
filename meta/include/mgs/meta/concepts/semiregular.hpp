#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/copyable.hpp>
#include <mgs/meta/concepts/default_constructible.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_semiregular
{
  using requirements = std::tuple<is_copyable<T>, is_default_constructible<T>>;

  static constexpr auto const value =
      is_copyable<T>::value && is_default_constructible<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::semiregular");
    return 1;
  }
};

template <typename T>
constexpr auto is_semiregular_v = is_semiregular<T>::value;

template <typename T, typename = std::enable_if_t<is_semiregular<T>::value>>
using semiregular = T;
}
}
