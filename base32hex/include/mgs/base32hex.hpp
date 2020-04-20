#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base32hex/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_codec.hpp>

namespace mgs
{
using base32hex = base_n::basic_codec<detail::base32hex_encoding_traits<>>;
}
