#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/concepts/totally_ordered.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/addition.hpp>
#include <mgs/meta/detected/operators/addition_assignment.hpp>
#include <mgs/meta/detected/operators/array_subscript.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/detected/operators/substraction_assignment.hpp>
#include <mgs/meta/iter_concept.hpp>
#include <mgs/meta/iter_difference_t.hpp>
#include <mgs/meta/iter_reference_t.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_random_access_iterator
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using difference_type = detected_t<iter_difference_t, T>;

  static constexpr auto const has_correct_category =
      is_derived_from<detected_t<meta::iter_concept, T>,
                      std::random_access_iterator_tag>::value;

  static constexpr auto const has_addition_assignment =
      is_detected_exact<lvalue_ref,
                        detected::operators::addition_assignment,
                        lvalue_ref,
                        difference_type const>::value;

  static constexpr auto const has_addition_t_dt =
      is_detected_exact<T,
                        detected::operators::addition,
                        T const,
                        difference_type const>::value;

  // for some reason, the following will return T const with MSVC
  static constexpr auto const has_addition_dt_t =
      is_detected_exact<T,
                        detected::operators::addition,
                        difference_type const,
                        T const>::value ||
      is_detected_exact<T const,
                        detected::operators::addition,
                        difference_type const,
                        T const>::value;

  static constexpr auto const has_substraction =
      is_detected_exact<T,
                        detected::operators::substraction,
                        T const,
                        difference_type const>::value;

  static constexpr auto const has_substraction_assignment =
      is_detected_exact<lvalue_ref,
                        detected::operators::substraction_assignment,
                        lvalue_ref,
                        difference_type const>::value;

  static constexpr auto const has_array_subscript =
      is_detected_exact<detected_t<meta::iter_reference_t, T>,
                        detected::operators::array_subscript,
                        T const,
                        difference_type const>::value;

public:
  using requirements = std::tuple<is_bidirectional_iterator<T>,
                                  is_totally_ordered<T>,
                                  is_sized_sentinel_for<T, T>>;

  static auto constexpr value =
      is_bidirectional_iterator<T>::value &&
      is_totally_ordered<T>::value && is_sized_sentinel_for<T, T>::value &&
      has_correct_category && has_addition_assignment && has_addition_t_dt &&
      has_addition_dt_t && has_substraction && has_substraction_assignment &&
      has_array_subscript;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::random_access_iterator");
    static_assert(has_correct_category,
                  "iterator category tag must derive from "
                  "std::random_access_iterator_tag");
    static_assert(has_addition_assignment,
                  "invalid or missing operator: 'T& "
                  "operator+=(meta::iter_difference_t<T>)'");
    static_assert(has_addition_t_dt,
                  "invalid or missing operator: 'T operator+(T const, "
                  "meta::iter_difference_t<T>)'");
    static_assert(has_addition_dt_t,
                  "invalid or missing operator: 'T "
                  "operator+(meta::iter_difference_t<T>, T "
                  "const)'");
    static_assert(has_substraction_assignment,
                  "invalid or missing operator: 'T "
                  "operator-=(meta::iter_difference_t<T>)'");
    static_assert(has_substraction,
                  "invalid or missing operator: 'T operator-(T const&, "
                  "meta::iter_difference_t<T>)'");
    static_assert(has_array_subscript,
                  "invalid or missing operator: "
                  "'meta::iter_reference_t<T> "
                  "operator[](meta::iter_difference_t<T>)'");
    return 1;
  }
};

template <typename T>
constexpr auto is_random_access_iterator_v =
    is_random_access_iterator<T>::value;

template <typename T,
          typename = std::enable_if_t<is_random_access_iterator<T>::value>>
using random_access_iterator = T;
}
}
