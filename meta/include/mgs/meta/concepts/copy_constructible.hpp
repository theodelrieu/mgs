#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/constructible_from.hpp>
#include <mgs/meta/concepts/convertible_to.hpp>
#include <mgs/meta/concepts/move_constructible.hpp> 
namespace mgs
{
namespace meta
{
template <typename T>
struct is_copy_constructible
{
private:
  using lvalue_ref = std::add_lvalue_reference_t<T>;
  using const_lvalue_ref = std::add_lvalue_reference_t<std::add_const_t<T>>;

public:
  using requirements = std::tuple<is_move_constructible<T>,
                                  is_constructible_from<T, lvalue_ref>,
                                  is_convertible_to<lvalue_ref, T>,
                                  is_constructible_from<T, const_lvalue_ref>,
                                  is_convertible_to<const_lvalue_ref, T>,
                                  is_constructible_from<T, std::add_const_t<T>>,
                                  is_convertible_to<std::add_const_t<T>, T>>;

  static constexpr auto const value =
      is_move_constructible<T>::value &&
      is_constructible_from<T, lvalue_ref>::value &&
      is_convertible_to<lvalue_ref, T>::value &&
      is_constructible_from<T, const_lvalue_ref>::value &&
      is_convertible_to<const_lvalue_ref, T>::value &&
      is_constructible_from<T, std::add_const_t<T>>::value &&
      is_convertible_to<std::add_const_t<T>, T>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model meta::copy_constructible");
    return 1;
  }
};

template <typename T>
constexpr auto is_copy_constructible_v = is_copy_constructible<T>::value;

template <typename T,
          typename = std::enable_if_t<is_copy_constructible<T>::value>>
using copy_constructible = T;
}
}
