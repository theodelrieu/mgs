#pragma once

namespace mgs
{
namespace base_n
{
namespace detected
{
namespace static_data_members
{
template <typename T>
using padding_policy = decltype(T::padding_policy);
}
}
}
}
