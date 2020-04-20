#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/detail/insert_iterator.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_input_or_output_iterator
{
  using requirements =
      std::tuple<is_dereferenceable<T>, is_weakly_incrementable<T>>;

  static constexpr auto const value =
      detail::is_insert_iterator<T>::value ||
      (is_weakly_incrementable<T>::value && is_dereferenceable<T>::value);

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::input_or_output_iterator");
    return 1;
  }
};

template <typename T>
constexpr auto is_input_or_output_iterator_v =
    is_input_or_output_iterator<T>::value;

template <typename T,
          typename = std::enable_if_t<is_input_or_output_iterator<T>::value>>
using input_or_output_iterator = T;
}
}
