#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/detail/call_std/begin.hpp>

namespace mgs
{
namespace meta
{
template <typename R, typename = std::enable_if_t<is_range<R>::value>>
using iterator_t = detail::result_of_begin<decltype(std::declval<R&>())>;
}
}
