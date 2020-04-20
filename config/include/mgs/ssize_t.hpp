#pragma once

#include <cstddef>
#include <type_traits>

namespace mgs
{
using ssize_t =
    std::common_type_t<std::ptrdiff_t, std::make_signed_t<std::size_t>>;
}
