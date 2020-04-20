#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/detail/call_std/end.hpp>

namespace mgs
{
namespace meta
{
template <typename R, typename = std::enable_if_t<is_range<R>::value>>
using sentinel_t = detail::result_of_end<decltype(std::declval<R&>())>;
}
}
