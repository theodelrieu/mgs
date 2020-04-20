#pragma once

#include <utility>

namespace mgs
{
namespace meta
{
namespace detected
{
namespace operators
{
template <typename T, typename U>
using array_subscript = decltype(std::declval<T>()[std::declval<U>()]);
}
}
}
}
