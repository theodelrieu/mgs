#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

namespace mgs
{
namespace meta
{
template <typename T, typename U>
struct is_equality_comparable_with
{
  using requirements = std::tuple<is_equality_comparable<T>,
                                  is_equality_comparable<U>,
                                  is_weakly_equality_comparable_with<T, U>>;

  static constexpr auto const value =
      is_equality_comparable<T>::value && is_equality_comparable<U>::value &&
      is_weakly_equality_comparable_with<T, U>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_equality_comparable_with::value,
                  "T does not model meta::equality_comparable_with<U>");
    return 1;
  }
};

template <typename T, typename U>
constexpr auto is_equality_comparable_with_v =
    is_equality_comparable_with<T, U>::value;

template <typename T,
          typename U,
          typename = std::enable_if_t<is_equality_comparable_with<T, U>::value>>
using equality_comparable_with = T;
}
}
