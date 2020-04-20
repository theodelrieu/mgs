#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/concepts/movable.hpp>
#include <mgs/meta/concepts/same_as.hpp>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/equality.hpp>
#include <mgs/meta/detected/operators/inequality.hpp>
#include <mgs/meta/detected/operators/logical_and.hpp>
#include <mgs/meta/detected/operators/logical_not.hpp>
#include <mgs/meta/detected/operators/logical_or.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_boolean
{
private:
  using unref_T = std::remove_reference_t<T>;
  using uncvref_T = std::remove_cv_t<unref_T>;
  using B = std::add_const_t<std::add_lvalue_reference_t<unref_T>>;

  static constexpr auto const is_convertible_to_bool =
      is_convertible_to<B, bool>::value;

  static constexpr auto const is_logical_not_convertible_to_bool =
      is_convertible_to<detected_t<detected::operators::logical_not, B>,
                        bool>::value;

  static constexpr auto const is_logical_and_boolean_bool_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_and, B, bool const>,
                 bool>::value;

  static constexpr auto const is_logical_or_boolean_bool_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_or, B, bool const>,
                 bool>::value;

  static constexpr auto const is_logical_and_boolean_boolean_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_and, B, B>,
                 bool>::value;

  static constexpr auto const is_logical_and_bool_boolean_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_and, bool const, B>,
                 bool>::value;

  static constexpr auto const is_logical_or_boolean_boolean_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_or, B, B>,
                 bool>::value;

  static constexpr auto const is_logical_or_bool_boolean_same_as_bool =
      is_same_as<detected_t<detected::operators::logical_or, bool const, B>,
                 bool>::value;

  static constexpr auto const is_equality_boolean_boolean_convertible_to_bool =
      is_convertible_to<detected_t<detected::operators::equality, B, B>,
                        bool>::value;

  static constexpr auto const is_equality_boolean_bool_convertible_to_bool =
      is_convertible_to<
          detected_t<detected::operators::equality, B, bool const>,
          bool>::value;

  static constexpr auto const is_equality_bool_boolean_convertible_to_bool =
      is_convertible_to<
          detected_t<detected::operators::equality, bool const, B>,
          bool>::value;

  static constexpr auto const
      is_inequality_boolean_boolean_convertible_to_bool =
          is_convertible_to<detected_t<detected::operators::inequality, B, B>,
                            bool>::value;

  static constexpr auto const is_inequality_boolean_bool_convertible_to_bool =
      is_convertible_to<
          detected_t<detected::operators::inequality, B, bool const>,
          bool>::value;

  static constexpr auto const is_inequality_bool_boolean_convertible_to_bool =
      is_convertible_to<
          detected_t<detected::operators::inequality, bool const, B>,
          bool>::value;

public:
  using requirements = std::tuple<is_movable<uncvref_T>>;

  static constexpr auto const value =
      is_movable<uncvref_T>::value && is_convertible_to_bool &&
      is_logical_not_convertible_to_bool &&
      is_logical_and_boolean_bool_same_as_bool &&
      is_logical_or_boolean_bool_same_as_bool &&
      is_logical_and_boolean_boolean_same_as_bool &&
      is_logical_and_bool_boolean_same_as_bool &&
      is_logical_or_boolean_boolean_same_as_bool &&
      is_logical_or_bool_boolean_same_as_bool &&
      is_equality_boolean_boolean_convertible_to_bool &&
      is_equality_boolean_bool_convertible_to_bool &&
      is_equality_bool_boolean_convertible_to_bool &&
      is_inequality_boolean_boolean_convertible_to_bool &&
      is_inequality_boolean_boolean_convertible_to_bool &&
      is_inequality_bool_boolean_convertible_to_bool;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::boolean");
    static_assert(is_convertible_to_bool,
                  "T{} does not model meta::convertible_to<bool>, operator "
                  "bool() must be "
                  "const, and not explicit");
    static_assert(is_logical_not_convertible_to_bool,
                  "!T{} does not model meta:: convertible_to<bool>");
    static_assert(is_logical_and_boolean_bool_same_as_bool,
                  "T{} && bool{} does not model meta::same_as<bool>");
    static_assert(is_logical_or_boolean_bool_same_as_bool,
                  "T{} || bool{} does not model meta::same_as<bool>");
    static_assert(is_logical_and_boolean_boolean_same_as_bool,
                  "T{} && T{} does not model meta::same_as<bool>");
    static_assert(is_logical_and_bool_boolean_same_as_bool,
                  "bool{} && T{} does not model meta::same_as<bool>");
    static_assert(is_logical_or_boolean_boolean_same_as_bool,
                  "T{} || T{} does not model meta::same_as<bool>");
    static_assert(is_logical_or_bool_boolean_same_as_bool,
                  "bool{} || T{} does not model meta::same_as<bool>");
    static_assert(is_equality_boolean_boolean_convertible_to_bool,
                  "T{} == T{} does not model meta::convertible_to<bool>");
    static_assert(is_equality_boolean_bool_convertible_to_bool,
                  "T{} == bool{} does not model meta::convertible_to<bool>");
    static_assert(is_equality_bool_boolean_convertible_to_bool,
                  "bool{} == T{} does not model meta::convertible_to<bool>");
    static_assert(is_inequality_boolean_boolean_convertible_to_bool,
                  "T{} != T{} does not model meta::convertible_to<bool>");
    static_assert(is_inequality_boolean_bool_convertible_to_bool,
                  "T{} != bool{} does not model meta::convertible_to<bool>");
    static_assert(is_inequality_bool_boolean_convertible_to_bool,
                  "bool{} != T{} does not model meta::convertible_to<bool>");
    return 1;
  }
};

template <typename T>
constexpr auto is_boolean_v = is_boolean<T>::value;

template <typename T,
          typename = std::enable_if_t<is_boolean<T>::value>>
using boolean = T;
}
}
