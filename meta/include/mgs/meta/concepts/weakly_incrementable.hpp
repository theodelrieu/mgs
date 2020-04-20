#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/detail/insert_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/signed_integral.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>
#include <mgs/meta/iter_difference_t.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct is_weakly_incrementable_impl
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using difference_type_t = meta::detected_t<meta::iter_difference_t, T>;

  static constexpr auto const has_pre_increment =
      meta::is_detected_exact<lvalue_ref,
                              meta::detected::operators::pre_increment,
                              lvalue_ref>::value;

  static constexpr auto const has_post_increment =
      meta::is_detected<meta::detected::operators::post_increment,
                        lvalue_ref>::value;

  static constexpr auto const has_signed_difference_type =
      meta::is_signed_integral<difference_type_t>::value;

public:
  using requirements = std::tuple<meta::is_semiregular<T>>;

  static constexpr auto const value = meta::is_semiregular<T>::value &&
                                      has_pre_increment && has_post_increment &&
                                      has_signed_difference_type;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::weakly_incrementable");
    static_assert(has_pre_increment,
                  "invalid or missing operator: 'T& operator++()'");
    static_assert(has_post_increment,
                  "invalid or missing operator: '/* any */ operator++(int)'");
    static_assert(has_signed_difference_type,
                  "meta::iter_difference_t<T> must model meta::signed_integral");
    return 1;
  }
};

template <typename T>
struct is_weakly_incrementable_impl<
    T,
    std::enable_if_t<is_insert_iterator<T>::value>> : std::true_type
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(
        is_weakly_incrementable_impl::value,
        "T is an insert_iterator, but somehow is not "
        "meta::weakly_incrementable... Most likely a bug, please report!");
    return 1;
  }
};
}

namespace meta
{
template <typename T>
struct is_weakly_incrementable : detail::is_weakly_incrementable_impl<T>
{
};

template <typename T>
constexpr auto is_weakly_incrementable_v = is_weakly_incrementable<T>::value;

template <typename T,
          typename = std::enable_if_t<is_weakly_incrementable<T>::value>>
using weakly_incrementable = T;
}
}
