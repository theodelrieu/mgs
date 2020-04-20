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
template <typename T>
using post_increment = decltype(std::declval<T>()++);
}
}
}
}
