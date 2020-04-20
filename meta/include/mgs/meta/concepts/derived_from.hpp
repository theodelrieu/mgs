#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/detected.hpp>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/regular.hpp>

namespace mgs
{
namespace detail
{
template <typename Derived, typename Base, typename = void>
struct is_derived_from_impl : std::false_type
{
};

// Derived must be a complete type.
template <typename Derived, typename Base>
struct is_derived_from_impl<
    Derived,
    Base,
    std::enable_if_t<meta::is_complete_type<Derived>::value>>
{
  static constexpr auto const value =
      std::is_base_of<Base, Derived>::value &&
      std::is_convertible<Derived const volatile*, Base const volatile*>::value;
};
}

namespace meta
{
template <typename Derived, typename Base>
struct is_derived_from : detail::is_derived_from_impl<Derived, Base>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_derived_from::value,
                  "Derived does not model meta::derived_from<Base>");
    return 1;
  }
};

template <typename Derived, typename Base>
constexpr auto is_derived_from_v = is_derived_from<Derived, Base>::value;

template <typename Derived,
          typename Base,
          typename = std::enable_if_t<is_derived_from<Derived, Base>::value>>
using derived_from = Derived;
}
}
