#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/boolean.hpp>
#include <mgs/meta/concepts/common_reference_with.hpp>
#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/equality_comparable_with.hpp>
#include <mgs/meta/concepts/totally_ordered.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>

namespace mgs
{
namespace meta
{
template <typename T, typename U>
struct is_totally_ordered_with
{
private:
  using t_const_lvalue_ref =
      std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>>;
  using u_const_lvalue_ref =
      std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<U>>>;

  static constexpr auto const has_less_than_t_u =
      is_boolean<detected_t<detected::operators::less_than,
                            t_const_lvalue_ref,
                            u_const_lvalue_ref>>::value;

  static constexpr auto const has_less_than_u_t =
      is_boolean<detected_t<detected::operators::less_than,
                            u_const_lvalue_ref,
                            t_const_lvalue_ref>>::value;

  static constexpr auto const has_less_or_equal_than_t_u =
      is_boolean<detected_t<detected::operators::less_or_equal_than,
                            t_const_lvalue_ref,
                            u_const_lvalue_ref>>::value;

  static constexpr auto const has_less_or_equal_than_u_t =
      is_boolean<detected_t<detected::operators::less_or_equal_than,
                            u_const_lvalue_ref,
                            t_const_lvalue_ref>>::value;

  static constexpr auto const has_greater_than_t_u =
      is_boolean<detected_t<detected::operators::greater_than,
                            t_const_lvalue_ref,
                            u_const_lvalue_ref>>::value;

  static constexpr auto const has_greater_than_u_t =
      is_boolean<detected_t<detected::operators::greater_than,
                            u_const_lvalue_ref,
                            t_const_lvalue_ref>>::value;

  static constexpr auto const has_greater_or_equal_than_t_u =
      is_boolean<detected_t<detected::operators::greater_or_equal_than,
                            t_const_lvalue_ref,
                            u_const_lvalue_ref>>::value;

  static constexpr auto const has_greater_or_equal_than_u_t =
      is_boolean<detected_t<detected::operators::greater_or_equal_than,
                            u_const_lvalue_ref,
                            t_const_lvalue_ref>>::value;

  static constexpr auto const has_common_reference_t_u =
      has_common_reference_with<t_const_lvalue_ref, u_const_lvalue_ref>::value;

  static constexpr auto const is_common_reference_totally_ordered =
      is_totally_ordered<detected_t<common_reference_t,
                                    t_const_lvalue_ref,
                                    u_const_lvalue_ref>>::value;

public:
  using requirements = std::tuple<
      is_totally_ordered<T>,
      is_totally_ordered<U>,
      has_common_reference_with<t_const_lvalue_ref, u_const_lvalue_ref>,
      is_totally_ordered<detected_t<common_reference_t,
                                    t_const_lvalue_ref,
                                    u_const_lvalue_ref>>,
      is_equality_comparable_with<T, U>>;

  static constexpr auto const value =
      is_totally_ordered<T>::value && is_totally_ordered<U>::value &&
      has_common_reference_t_u && is_common_reference_totally_ordered &&
      is_equality_comparable_with<T, U>::value && has_less_than_t_u &&
      has_less_than_u_t && has_less_or_equal_than_t_u &&
      has_less_or_equal_than_u_t && has_greater_than_t_u &&
      has_greater_than_u_t && has_greater_or_equal_than_t_u &&
      has_greater_or_equal_than_u_t;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_totally_ordered_with::value,
                  "T does not model meta::totally_ordered_with<U>");

    static_assert(
        has_less_than_t_u,
        "invalid or missing operator: 'meta::boolean operator<(T const&, U const&)'");
    static_assert(
        has_less_than_u_t,
        "invalid or missing operator: 'meta::boolean operator<(U const&, T const&)'");
    static_assert(
        has_less_or_equal_than_t_u,
        "invalid or missing operator: 'meta::boolean operator<=(T const&, U const&)'");
    static_assert(
        has_less_or_equal_than_u_t,
        "invalid or missing operator: 'meta::boolean operator<=(U const&, T const&)'");
    static_assert(
        has_greater_than_t_u,
        "invalid or missing operator: 'meta::boolean operator>(T const&, U const&)'");
    static_assert(
        has_greater_than_u_t,
        "invalid or missing operator: 'meta::boolean operator>(U const&, T const&)'");
    static_assert(
        has_greater_or_equal_than_t_u,
        "invalid or missing operator: 'meta::boolean operator>=(T const&, U const&)'");
    static_assert(
        has_greater_or_equal_than_t_u,
        "invalid or missing operator: 'meta::boolean operator>=(U const&, T const&)'");
    return 1;
  }
};

template <typename T, typename U>
constexpr auto is_totally_ordered_with_v = is_totally_ordered_with<T, U>::value;

template <typename T,
          typename U,
          typename = std::enable_if_t<is_totally_ordered_with<T, U>::value>>
using totally_ordered_with = T;
}
}
