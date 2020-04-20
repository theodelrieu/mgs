#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base64/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_codec.hpp>

namespace mgs
{
using base64 = base_n::basic_codec<detail::base64_encoding_traits<>>;
}
