#pragma once

#include <mgs/meta/detail/common_reference_impl.hpp>

namespace mgs
{
namespace meta
{
template <typename... Ts>
struct common_reference : detail::common_reference<Ts...>
{
};

template <typename... Ts>
using common_reference_t = typename common_reference<Ts...>::type;
}
}
