#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base64url/detail/basic_encoding_traits.hpp>
#include <mgs/base_n/basic_codec.hpp>

namespace mgs
{
using base64url = base_n::basic_codec<
    detail::basic_base64url_encoding_traits<base_n::padding_policy::required>>;

using base64url_nopad = base_n::basic_codec<
    detail::basic_base64url_encoding_traits<base_n::padding_policy::none>,
    detail::basic_base64url_encoding_traits<base_n::padding_policy::optional>>;
}
