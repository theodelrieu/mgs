#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/detail/call_std/swap.hpp>

namespace mgs
{
namespace detail
{
// This mess is required since std::swap only got SFINAE-correctness in C++17
// https://stackoverflow.com/a/26745591
template <typename T, typename = void>
struct is_swappable_impl : std::false_type
{
};

template <typename T>
struct is_swappable_impl<T,
                         std::enable_if_t<meta::is_complete_type<T>::value &&
                                          !std::is_array<T>::value>>
  : std::integral_constant<
        bool,
        meta::is_detected<detail::result_of_swap, T&, T&>::value &&
            (!would_call_std_swap<T&, T&>::value ||
             (std::is_move_assignable<T>::value &&
              std::is_move_constructible<T>::value))>
{
};

template <typename T, std::size_t N>
struct is_swappable_impl<T[N]> : is_swappable_impl<T>
{
};
}

namespace meta
{
template <typename T>
struct is_swappable : detail::is_swappable_impl<std::remove_reference_t<T>>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_swappable::value, "T does not model meta::swappable");
    return 1;
  }
};

template <typename T>
constexpr auto is_swappable_v = is_swappable<T>::value;

template <typename T, typename = std::enable_if_t<is_swappable<T>::value>>
using swappable = T;
}
}
