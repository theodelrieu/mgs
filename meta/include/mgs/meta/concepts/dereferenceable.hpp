#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T>
using with_reference = T&;

template <typename T, typename = void>
struct can_reference : std::false_type
{
};

template <typename T>
struct can_reference<T, meta::void_t<with_reference<T>>> : std::true_type
{
};

template <typename T, typename = void>
struct is_dereferenceable_impl : std::false_type
{
};

template <typename T>
struct is_dereferenceable_impl<
    T,
    std::enable_if_t<
        meta::is_detected<meta::detected::operators::dereference, T>::value>>
{
  static constexpr auto const value =
      can_reference<meta::detected::operators::dereference<T>>::value;
};
}

namespace meta
{
template <typename T>
struct is_dereferenceable: detail::is_dereferenceable_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_dereferenceable::value, "T does not model meta::dereferenceable");
    return 1;
  }
};

template <typename T>
constexpr auto is_dereferenceable_v = is_dereferenceable<T>::value;

template <typename T, typename = std::enable_if_t<is_dereferenceable<T>::value>>
using dereferenceable = T;
}
}
