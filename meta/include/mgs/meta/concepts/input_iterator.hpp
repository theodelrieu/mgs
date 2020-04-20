#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/readable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_concept.hpp>

// https://en.cppreference.com/w/cpp/experimental/ranges/InputIterator

namespace mgs
{
namespace meta
{
template <typename T>
struct is_input_iterator
{
private:
  static constexpr auto const has_correct_category =
      is_derived_from<detected_t<meta::iter_concept, T>,
                      std::input_iterator_tag>::value;

public:
  using requirements =
      std::tuple<is_input_or_output_iterator<T>, is_readable<T>>;

  static constexpr auto const value = has_correct_category &&
                                      is_input_or_output_iterator<T>::value &&
                                      is_readable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::input_iterator");
    static_assert(
        has_correct_category,
        "iterator category tag must derive from std::input_iterator_tag");
    return 1;
  }
};

template <typename T>
constexpr auto is_input_iterator_v = is_input_iterator<T>::value;

template <typename T, typename = std::enable_if_t<is_input_iterator<T>::value>>
using input_iterator = T;
}
}
