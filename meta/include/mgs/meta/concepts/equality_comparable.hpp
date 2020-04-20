#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_equality_comparable::value,
                  "T does not model meta::equality_comparable");
    return 1;
  }
};

template <typename T>
constexpr auto is_equality_comparable_v = is_equality_comparable<T>::value;

template <typename T,
          typename = std::enable_if_t<is_equality_comparable<T>::value>>
using equality_comparable = T;
}
}
