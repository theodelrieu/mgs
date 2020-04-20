#pragma once

#include <type_traits>

#include <mgs/meta/concepts/bidirectional_range.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_random_access_range
{
private:
  using Iterator = meta::detected_t<iterator_t, T>;

public:
  using requirements = std::tuple<is_bidirectional_range<T>,
                                  is_random_access_iterator<Iterator>>;

  static constexpr bool value = is_bidirectional_range<T>::value &&
                                is_random_access_iterator<Iterator>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::random_access_range");
    return 1;
  }
};

template <typename T>
constexpr auto is_random_access_range_v = is_random_access_range<T>::value;

template <typename T, typename = std::enable_if_t<is_random_access_range_v<T>>>
using random_access_range = T;
}
}
