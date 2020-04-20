#pragma once

#include <cstddef>

namespace mgs
{
namespace meta
{
template <std::size_t N>
struct priority_tag : priority_tag<N - 1>
{
};

template <>
struct priority_tag<0>
{
};
}
}
