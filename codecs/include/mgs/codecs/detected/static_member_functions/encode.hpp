#pragma once

#include <mgs/meta/detected/macros.hpp>

namespace mgs
{
namespace codecs
{
namespace detected
{
namespace static_member_functions
{
MGS_META_DETECTED_STATIC_MEMBER_FUNCTION(encode);

template <typename T, typename Ret, typename... Args>
using encode_tpl = decltype(T::template encode<Ret>(std::declval<Args>()...));
}
}
}
}
