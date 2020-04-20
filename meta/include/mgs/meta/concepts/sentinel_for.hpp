#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

namespace mgs
{
namespace meta
{
template <typename T, typename Iterator>
struct is_sentinel_for
  : std::integral_constant<
        bool,
        is_semiregular<T>::value &&
            is_input_or_output_iterator<Iterator>::value &&
            is_weakly_equality_comparable_with<T, Iterator>::value>
{
  using requirements =
      std::tuple<is_semiregular<T>,
                 is_input_or_output_iterator<Iterator>,
                 is_weakly_equality_comparable_with<T, Iterator>>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_sentinel_for::value,
                  "T does not model meta::sentinel_for<Iterator>");
    return 1;
  }
};

template <typename T, typename Iterator>
constexpr auto is_sentinel_for_v = is_sentinel_for<T, Iterator>::value;

template <typename T,
          typename Iterator,
          typename = std::enable_if_t<is_sentinel_for<T, Iterator>::value>>
using sentinel_for = T;
}
}
