#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace detail
{
// Workaround a GCC bug with operator-(void*, void*) in unevaluated context
template <typename T, typename U, typename = void>
struct substraction_impl
{
};

template <typename T, typename U>
struct substraction_impl<
    T,
    U,
    meta::void_t<decltype(std::declval<T>() - std::declval<U>())>>
{
  using type = decltype(std::declval<T>() - std::declval<U>());
};

template <typename T, typename U, typename, typename>
struct substraction : substraction_impl<T, U>
{
};

template <typename T, typename U>
struct substraction<T, U, void*, void*>
{
};
}

namespace meta
{
namespace detected
{
namespace operators
{
template <typename T, typename U>
using substraction = typename detail::substraction<
    T,
    U,
    std::remove_cv_t<std::remove_reference_t<T>>,
    std::remove_cv_t<std::remove_reference_t<U>>>::type;
}
}
}
}
