#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/common_reference_with.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_reference_t.hpp>
#include <mgs/meta/iter_rvalue_reference_t.hpp>
#include <mgs/meta/iter_value_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct is_readable_impl : std::false_type
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(meta::is_detected<meta::iter_value_t, T>::value,
                  "meta::iter_value_t<T> is ill-formed");
    static_assert(meta::is_detected<meta::iter_reference_t, T>::value,
                  "meta::iter_reference_t<T> is ill-formed");
    static_assert(meta::is_detected<meta::iter_rvalue_reference_t, T>::value,
                  "meta::iter_rvalue_reference_t<T> is ill-formed");
    return 1;
  }
};

template <typename T>
struct is_readable_impl<
    T,
    // void_t does not work here for MSVC
    std::enable_if_t<
        meta::is_detected<meta::iter_value_t, T>::value &&
        meta::is_detected<meta::iter_reference_t, T>::value &&
        meta::is_detected<meta::iter_rvalue_reference_t, T>::value>>
{
  using requirements = std::tuple<
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_reference_t<T>>,
          std::add_lvalue_reference_t<meta::iter_value_t<T>>>,
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_reference_t<T>>,
          std::add_rvalue_reference_t<meta::iter_rvalue_reference_t<T>>>,
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_rvalue_reference_t<T>>,
          std::add_lvalue_reference_t<
              std::add_const_t<meta::iter_value_t<T>>>>>;

  static constexpr auto const value =
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_reference_t<T>>,
          std::add_lvalue_reference_t<meta::iter_value_t<T>>>::value &&
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_reference_t<T>>,
          std::add_rvalue_reference_t<meta::iter_rvalue_reference_t<T>>>::
          value &&
      meta::has_common_reference_with<
          std::add_rvalue_reference_t<meta::iter_rvalue_reference_t<T>>,
          std::add_lvalue_reference_t<
              std::add_const_t<meta::iter_value_t<T>>>>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::readable");
    return 1;
  }
};
}

namespace meta
{
template <typename T>
struct is_readable : detail::is_readable_impl<T>
{
};

template <typename T>
constexpr auto is_readable_v = is_readable<T>::value;

template <typename T, typename = std::enable_if_t<is_readable<T>::value>>
using readable = T;
}
}
