#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/concepts/regular.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
struct is_incrementable
{
  using requirements = std::tuple<is_regular<T>, is_weakly_incrementable<T>>;
  using lvalue_ref = std::add_lvalue_reference_t<T>;

  static constexpr auto const has_post_increment =
      is_detected_exact<T, detected::operators::post_increment, lvalue_ref>::
          value;

  static constexpr auto const value = is_regular<T>::value &&
                                      has_post_increment &&
                                      is_weakly_incrementable<T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::incrementable");
    static_assert(has_post_increment,
                  "invalid or missing operator: 'T operator++(int)'");
    return 1;
  }
};

template <typename T>
constexpr auto is_incrementable_v = is_incrementable<T>::value;

template <typename T, typename = std::enable_if_t<is_incrementable<T>::value>>
using incrementable = T;
}
}
