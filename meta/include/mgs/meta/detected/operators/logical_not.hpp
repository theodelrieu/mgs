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
using logical_not = decltype(!std::declval<T>());
}
}
}
}
