#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace detail
{
// helpers to implement common_reference without going mad
// taken from the standard
template <typename X, typename Y>
using cond_res =
    decltype(false ? std::declval<X (&)()>()() : std::declval<Y (&)()>()());

template <typename From, typename To>
struct copycv_impl
{
  using type = To;
};

template <typename From, typename To>
struct copycv_impl<From const, To>
{
  using type = To const;
};

template <typename From, typename To>
struct copycv_impl<From volatile, To>
{
  using type = To volatile;
};

template <typename From, typename To>
struct copycv_impl<From const volatile, To>
{
	using type = To const volatile;
};

template <typename From, typename To>
using copycv = typename copycv_impl<From, To>::type;

template <typename T, bool B = std::is_reference<T>::value>
struct common_ref_bullet_one_impl
{
};

template <typename T>
struct common_ref_bullet_one_impl<T, true>
{
  using type = T;
};

template <typename X, typename Y>
struct common_ref_bullet_one
  : common_ref_bullet_one_impl<
        meta::detected_t<cond_res, copycv<X, Y>&, copycv<Y, X>&>>
{
};

template <typename A,
          typename B,
          typename C,
          bool b = std::is_convertible<A, C>::value&&
              std::is_convertible<B, C>::value>
struct common_ref_bullet_two_impl
{
};

template <typename A, typename B, typename C>
struct common_ref_bullet_two_impl<A, B, C, true>
{
  using type = C;
};

template <typename A, typename B, typename C>
struct common_ref_bullet_two : common_ref_bullet_two_impl<A, B, C>
{
};

template <typename A,
          typename D,
          bool b = std::is_convertible<A, D>::value>
struct common_ref_bullet_three_impl
{
};

template <typename A, typename D>
struct common_ref_bullet_three_impl<A, D, true>
{
  using type = D;
};

template <typename A, typename D>
struct common_ref_bullet_three : common_ref_bullet_three_impl<A, D>
{
};

template <typename A, typename B>
struct common_ref_impl;

template <typename X, typename Y>
struct common_ref_impl<X&, Y&> : common_ref_bullet_one<X, Y>
{
};

template <typename X, typename Y>
struct common_ref_impl<X&&, Y&&>
  : common_ref_bullet_two<
        X&&,
        Y&&,
        std::remove_reference_t<typename common_ref_impl<X&, Y&>::type>&&>
{
};


template <typename A, typename B>
using common_ref = typename common_ref_impl<A, B>::type;

template <typename X, typename Y>
struct common_ref_impl<X&&, Y&>
  : common_ref_bullet_three<X&&, meta::detected_t<common_ref, X const&, Y&>>
{
};

template <typename X, typename Y>
struct common_ref_impl<X&, Y&&> : common_ref_impl<Y&&, X&>
{
};

// common_reference bullets
template <typename T1, typename T2, bool B = false>
struct common_reference_bullet_one_impl : std::false_type
{
};

template <typename T1, typename T2>
struct common_reference_bullet_one_impl<
    T1,
    T2,
	true>
  : std::true_type
{
  using type = common_ref<T1, T2>;
};

template <typename T1, typename T2>
struct common_reference_bullet_one : common_reference_bullet_one_impl<T1, T2, std::is_reference<T1>::value &&
	std::is_reference<T2>::value &&
	meta::is_detected<common_ref, T1, T2>::value>
{
};

// skipping bullet two, since there is no customization point
// basic_common_reference before C++20

template <typename T1, typename T2, typename = void>
struct common_reference_bullet_three_impl : std::false_type
{
};

template <typename T1, typename T2>
struct common_reference_bullet_three_impl<
    T1,
    T2,
    std::enable_if_t<meta::is_detected<cond_res, T1, T2>::value>>
  : std::true_type
{
  using type = cond_res<T1, T2>;
};

template <typename T1, typename T2>
struct common_reference_bullet_three
  : common_reference_bullet_three_impl<T1, T2>
{
};

template <typename T1, typename T2, typename = void>
struct common_reference_bullet_four_impl : std::false_type
{
};

template <typename T1, typename T2>
struct common_reference_bullet_four_impl<
    T1,
    T2,
    std::enable_if_t<meta::is_detected<std::common_type_t, T1, T2>::value>>
  : std::true_type
{
  using type = std::common_type_t<T1, T2>;
};

template <typename T1, typename T2>
struct common_reference_bullet_four: common_reference_bullet_four_impl<T1, T2>
{
};

template <typename BulletOne,
          typename BulletThree,
          typename BulletFour,
          bool B1 = BulletOne::value,
          bool B3 = BulletThree::value,
          bool B4 = BulletFour::value>
struct common_reference_bullets_impl
{
};

template <typename BulletOne,
          typename BulletThree,
          typename BulletFour,
          bool B3,
          bool B4>
struct common_reference_bullets_impl<BulletOne,
                                     BulletThree,
                                     BulletFour,
                                     true,
                                     B3,
                                     B4>
{
  using type = typename BulletOne::type;
};

template <typename BulletOne,
          typename BulletThree,
          typename BulletFour,
          bool B4>
struct common_reference_bullets_impl<BulletOne,
                                     BulletThree,
                                     BulletFour,
                                     false,
                                     true,
                                     B4>
{
  using type = typename BulletThree::type;
};

template <typename BulletOne, typename BulletThree, typename BulletFour>
struct common_reference_bullets_impl<BulletOne,
                                     BulletThree,
                                     BulletFour,
                                     false,
                                     false,
                                     true>
{
  using type = typename BulletFour::type;
};

template <typename...>
struct common_reference
{
};

template <>
struct common_reference<>
{
};

template <typename T0>
struct common_reference<T0>
{
  using type = T0;
};

template <typename T1, typename T2>
struct common_reference<T1, T2>
  : common_reference_bullets_impl<common_reference_bullet_one<T1, T2>,
                                  common_reference_bullet_three<T1, T2>,
                                  common_reference_bullet_four<T1, T2>>
{
};

template <typename T1, typename T2, typename... Rest>
struct common_reference<T1, T2, Rest...>
  : common_reference<typename common_reference<T1, T2>::type, Rest...>
{
};
}
}
