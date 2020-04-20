#pragma once

#include <cstddef>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/detail/input_sanitizer.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          base_n::padding_policy = EncodingTraits::padding_policy>
struct max_decoded_size
{
  constexpr mgs::ssize_t operator()(mgs::ssize_t encoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    if (invalid_padding_character_pos<BitshiftTraits>(encoded_size))
      return -1;
    return (encoded_size / BitshiftTraits::nb_encoded_bytes *
            BitshiftTraits::nb_decoded_bytes) +
           (((encoded_size % BitshiftTraits::nb_encoded_bytes) *
             BitshiftTraits::nb_decoded_bytes) /
            BitshiftTraits::nb_encoded_bytes);
  }
};

template <typename EncodingTraits>
struct max_decoded_size<EncodingTraits,
                        base_n::padding_policy::required>
{
  constexpr mgs::ssize_t operator()(mgs::ssize_t encoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    if (encoded_size % BitshiftTraits::nb_encoded_bytes)
      return -1;
    return (encoded_size / BitshiftTraits::nb_encoded_bytes) *
           BitshiftTraits::nb_decoded_bytes;
  }
};
}
}
