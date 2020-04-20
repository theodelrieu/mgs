#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>

namespace mgs
{
namespace detail
{
// MSVC triggers an error if an incomplete type is passed to
// std::is_nothrow_destructible
template <typename T, typename = void>
struct is_destructible_impl : std::false_type
{
};

template <typename T>
struct is_destructible_impl<T,
                            std::enable_if_t<meta::is_complete_type<T>::value>>
{
  static constexpr auto const value = std::is_nothrow_destructible<T>::value;
};
}

namespace meta
{
template <typename T>
struct is_destructible : detail::is_destructible_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_destructible::value,
                  "T does not model meta::destructible");
    return 1;
  }
};

template <typename T>
constexpr auto is_destructible_v = is_destructible<T>::value;

template <typename T, typename = std::enable_if_t<is_destructible<T>::value>>
using destructible = T;
}
}
