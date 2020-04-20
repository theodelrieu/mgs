#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected/operators/dereference.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename U = meta::detected::operators::dereference<T>>
auto iter_move(T&& t)
    -> std::conditional_t<std::is_lvalue_reference<U>::value,
                          decltype(std::move(std::declval<U>())),
                          U>;
}

namespace meta
{
template <typename T>
using iter_rvalue_reference_t = decltype(detail::iter_move(std::declval<T&>()));
}
}
