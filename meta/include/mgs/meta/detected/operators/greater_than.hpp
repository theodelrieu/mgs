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
using greater_than = decltype(std::declval<T>() > std::declval<U>());
}
}
}
}
