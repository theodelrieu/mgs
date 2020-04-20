#pragma once

#include <iterator>
#include <type_traits>

#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/forward_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_decrement.hpp>
#include <mgs/meta/detected/operators/pre_decrement.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/iter_concept.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_bidirectional_iterator
{
  using requirements = std::tuple<is_forward_iterator<T>>;
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_correct_category =
      is_derived_from<detected_t<meta::iter_concept, T>,
                      std::bidirectional_iterator_tag>::value;

  static constexpr auto const has_pre_decrement =
      is_detected_exact<lvalue_ref,
                        detected::operators::pre_decrement,
                        lvalue_ref>::value;

  static constexpr auto const has_post_decrement =
      is_detected_exact<T, detected::operators::post_decrement, lvalue_ref>::
          value;

  static constexpr auto const value = is_forward_iterator<T>::value &&
                                      has_correct_category &&
                                      has_pre_decrement && has_post_decrement;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::bidirectional_iterator");
    static_assert(has_correct_category,
                  "iterator category tag must derive from "
                  "std::bidirectional_iterator_tag");
    static_assert(has_pre_decrement,
                  "invalid or missing operator: 'T& operator--()'");
    static_assert(has_post_decrement,
                  "invalid or missing operator: 'T operator--(int)'");
    return 1;
  }
};

template <typename T>
constexpr auto is_bidirectional_iterator_v =
    is_bidirectional_iterator<T>::value;

template <typename T,
          typename = std::enable_if_t<is_bidirectional_iterator<T>::value>>
using bidirectional_iterator = T;
}
}
