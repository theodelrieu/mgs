#pragma once

#include <type_traits>

#include <mgs/meta/concepts/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/forward_range.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_bidirectional_range
{
private:
  using Iterator = meta::detected_t<iterator_t, T>;

public:
  using requirements =
      std::tuple<is_forward_range<T>, is_bidirectional_iterator<Iterator>>;

  static constexpr bool value =
      is_forward_range<T>::value && is_bidirectional_iterator<Iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::bidirectional_range");
    return 1;
  }
};

template <typename T>
constexpr auto is_bidirectional_range_v = is_bidirectional_range<T>::value;

template <typename T, typename = std::enable_if_t<is_bidirectional_range_v<T>>>
using bidirectional_range = T;
}
}
