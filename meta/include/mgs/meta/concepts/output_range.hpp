#pragma once

#include <tuple>

#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
namespace meta
{
template <typename R, typename T>
struct is_output_range
{
private:
  using Iterator = meta::detected_t<iterator_t, R>;

public:
  using requirements = std::tuple<is_range<R>, is_output_iterator<Iterator, T>>;

  static constexpr bool value =
      is_range<R>::value && is_output_iterator<Iterator, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "R does not model meta::output_range");
    return 1;
  }
};

template <typename R, typename T>
constexpr auto is_output_range_v = is_output_range<R, T>::value;

template <typename R,
          typename T,
          typename = std::enable_if_t<is_output_range_v<R, T>>>
using output_range = R;
}
}
