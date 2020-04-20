#pragma once

#include <type_traits>

#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_common_range
{
private:
  static constexpr bool has_same_iterator_sentinel =
      std::is_same<iterator_t<T>, sentinel_t<T>>::value;

public:
  using requirements = std::tuple<is_range<T>>;

  static constexpr bool value =
      is_range<T>::value && has_same_iterator_sentinel;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::common_range");
    static_assert(has_same_iterator_sentinel,
                  "iterator and sentinel types must be the same");
    return 1;
  }
};

template <typename T>
constexpr auto is_common_range_v = is_common_range<T>::value;

template <typename T, typename = std::enable_if_t<is_common_range_v<T>>>
using common_range = T;
}
}
