#pragma once

#include <utility>

#include <mgs/meta/detected/operators/dereference.hpp>

namespace mgs
{
namespace meta
{
template <typename T>
using iter_reference_t = detected::operators::dereference<T&>;
}
}
