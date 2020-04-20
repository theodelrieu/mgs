#pragma once

#include <type_traits>

#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detail/call_std/begin.hpp>
#include <mgs/meta/detail/call_std/end.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_range
{
private:
  using t_ref = std::add_lvalue_reference_t<T>;
  using Iterator = meta::detected_t<detail::result_of_begin, t_ref>;
  using Sentinel = meta::detected_t<detail::result_of_end, t_ref>;

  static constexpr auto const has_begin = !std::is_same<Iterator, meta::nonesuch>::value;
  static constexpr auto const has_end = !std::is_same<Sentinel, meta::nonesuch>::value;

  static constexpr auto const is_iterator_begin =
      is_input_or_output_iterator<Iterator>::value;
  static constexpr auto const is_sentinel_end =
      is_sentinel_for<Sentinel, Iterator>::value;

public:
  static constexpr bool value = is_iterator_begin && is_sentinel_end;

  // do not trigger loads of static asserts when no function begin/end is found...
  using requirements = decltype(std::tuple_cat(
      std::conditional_t<
          has_begin,
          std::tuple<meta::is_input_or_output_iterator<Iterator>>,
          std::tuple<>>{},
      std::conditional_t<has_begin && has_end,
                         std::tuple<is_sentinel_for<Sentinel, Iterator>>,
                         std::tuple<>>{}));

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::range");
    static_assert(
        has_begin,
        "there is no available 'begin' member/free function for T");
    static_assert(has_end,
                  "there is no available 'end' member/free function for T");

    // only trigger if there is a begin function
    static_assert(!has_begin || is_iterator_begin,
                  "invalid 'begin' member/free function for T: does not return "
                  "a type that models meta::input_or_output_iterator");

    // only trigger if there is a begin and an end function.
    static_assert(
        (!has_begin || is_iterator_begin) || !has_end || is_sentinel_end,
        "invalid 'end' member/free function for T: does not return "
        "a type that models meta::sentinel_for<meta::iterator_t<T>>");
    return 1;
  }
};

template <typename T>
constexpr auto is_range_v = is_range<T>::value;

template <typename T, typename = std::enable_if_t<is_range_v<T>>>
using range = T;
}
}
