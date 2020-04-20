#pragma once

#include <cstddef>

#include <mgs/base_n/detail/math.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits>
struct bitshift_traits
{
  static constexpr auto nb_index_bits =
      log2<sizeof(EncodingTraits::alphabet)>();
  static constexpr auto nb_decoded_bytes =
      decoded_bytes<sizeof(EncodingTraits::alphabet)>();
  static constexpr auto nb_encoded_bytes =
      encoded_bytes<sizeof(EncodingTraits::alphabet)>();
  static constexpr auto nb_encoded_bits = nb_encoded_bytes * 8;
  static constexpr auto nb_decoded_bits = nb_decoded_bytes * 8;
  static constexpr auto encoded_shift = nb_decoded_bits - nb_index_bits;
  static constexpr auto decoded_shift = nb_decoded_bits - 8;
  static constexpr auto nb_max_padding_bytes =
      nb_encoded_bytes - ((8 / nb_index_bits) + ((8 % nb_index_bits) > 0));
};
}
}
