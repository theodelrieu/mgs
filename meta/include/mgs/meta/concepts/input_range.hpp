#pragma once

#include <type_traits>

#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_input_range
{
private:
  using Iterator = meta::detected_t<iterator_t, T>;

public:
  using requirements = std::tuple<is_range<T>, is_input_iterator<Iterator>>;

  static constexpr bool value =
      is_range<T>::value && is_input_iterator<Iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::input_range");
    return 1;
  }
};

template <typename T>
constexpr auto is_input_range_v = is_input_range<T>::value;

template <typename T, typename = std::enable_if_t<is_input_range_v<T>>>
using input_range = T;
}
}
