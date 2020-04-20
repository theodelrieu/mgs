#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T>
struct is_valid_is_convertible_to_type
{
  static constexpr auto const value =
      meta::is_complete_type<T>::value ||
      // int (&) [] is an incomplete type, and a reference type, so
      // std::is_array returns false without std::remove_reference_t
      std::is_array<std::remove_reference_t<T>>::value ||
      std::is_void<T>::value;
};

template <typename From, typename To>
using explicitly_convertible =
    decltype(static_cast<To>(std::declval<From (&)()>()()));

template <typename From, typename To, typename = void>
struct is_convertible_to_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    static_assert(
        is_valid_is_convertible_to_type<From>::value,
        "From must be a complete type, an array of unknown bounds, or void");
    static_assert(
        is_valid_is_convertible_to_type<To>::value,
        "To must be a complete type, an array of unknown bounds, or void");
    return 1;
  }
};

template <typename From, typename To>
struct is_convertible_to_impl<
    From,
    To,
    std::enable_if_t<is_valid_is_convertible_to_type<From>::value &&
                     is_valid_is_convertible_to_type<To>::value>>
{
private:
  static constexpr auto const is_implicitly_convertible =
      std::is_convertible<From, To>::value;

  // workaround arrays of unknown bound
  // using T = int[];
  // using U = int(&&)[];
  // 'T f()' is invalid, whereas 'U f()' is valid
  static constexpr auto const is_explicitly_convertible =
      meta::is_detected<explicitly_convertible,
                        std::add_rvalue_reference_t<From>,
                        To>::value;

public:
  static constexpr auto const value =
      is_implicitly_convertible && is_explicitly_convertible;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_implicitly_convertible,
                  "From must be implicitly convertible to To");
    static_assert(is_explicitly_convertible,
                  "From must be explicitly convertible to To");
    return 1;
  }
};
}

namespace meta
{
template <typename From, typename To>
struct is_convertible_to : detail::is_convertible_to_impl<From, To>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_convertible_to::value,
                  "From does not model meta::convertible_to<To>");

    return detail::is_convertible_to_impl<From, To>::trigger_static_asserts();
  }
};

template <typename From, typename To>
constexpr auto is_convertible_to_v = is_convertible_to<From, To>::value;

template <typename From,
          typename To,
          typename = std::enable_if_t<is_convertible_to<From, To>::value>>
using convertible_to = From;
}
}
