#pragma once

// This concept does not exist in C++20.

#include <tuple>
#include <type_traits>

#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct is_complete_type_impl : std::false_type
{
};

template <typename T>
struct is_complete_type_impl<T, meta::void_t<decltype(sizeof(T))>>
  : std::true_type
{
};
}

namespace meta
{
template <typename T>
struct is_complete_type : detail::is_complete_type_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_complete_type::value,
                  "T does not model meta::complete_type");
    return 1;
  }
};

template <typename T>
constexpr auto is_complete_type_v = is_complete_type<T>::value;

template <typename T, typename = std::enable_if_t<is_complete_type<T>::value>>
using complete_type = T;
}
}
