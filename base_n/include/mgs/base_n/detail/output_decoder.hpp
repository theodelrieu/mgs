#pragma once

#include <bitset>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/detail/math.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename decodingTraits>
struct output_decoder
{
  using BitshiftTraits = bitshift_traits<decodingTraits>;

  template <typename OutputIterator>
  static void decode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out,
      mgs::ssize_t n)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(255);

    for (auto i = 0; i < n; ++i)
    {
      out[i] = static_cast<unsigned char>(
          (decoded_bits >> static_cast<std::size_t>(
                               BitshiftTraits::decoded_shift - (8 * i)) &
           mask)
              .to_ulong());
    }
  }

  template <typename OutputIterator>
  static void decode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(255);

    for (auto i = 0; i < BitshiftTraits::nb_decoded_bytes; ++i)
    {
      out[i] = static_cast<unsigned char>(
          (decoded_bits >> static_cast<std::size_t>(
                               BitshiftTraits::decoded_shift - (8 * i)) &
           mask)
              .to_ulong());
    }
  }
};
}
}
