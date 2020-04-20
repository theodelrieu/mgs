#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/writable.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace detail
{
template <typename I, typename T>
auto _output_iterator_requirement(I i, T&& t)
    -> decltype(*i++ = std::forward<T>(t));

template <typename I, typename T>
using output_iterator_requirement = decltype(
    _output_iterator_requirement(std::declval<I>(), std::declval<T>()));
}

namespace meta
{
template <typename I, typename T>
struct is_output_iterator
{
  using requirements =
      std::tuple<is_input_or_output_iterator<I>, is_writable<I, T>>;

  static constexpr auto const value =
      is_input_or_output_iterator<I>::value && is_writable<I, T>::value &&
      is_detected<detail::output_iterator_requirement, I, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "I does not model meta::output_iterator<T>");
    return 1;
  }
};

template <typename I, typename T>
constexpr auto is_output_iterator_v = is_output_iterator<I, T>::value;

template <typename I,
          typename T,
          typename = std::enable_if_t<is_output_iterator<I, T>::value>>
using output_iterator = I;
}
}
