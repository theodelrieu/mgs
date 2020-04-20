#pragma once

#include <bitset>
#include <cstddef>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/detail/math.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits>
struct output_encoder
{
  using BitshiftTraits = bitshift_traits<EncodingTraits>;

  template <typename OutputIterator>
  static void encode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out,
      mgs::ssize_t n)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(
        pow<2, BitshiftTraits::nb_index_bits>() - 1);

    for (auto i = 0; i < n; ++i)
    {
      auto const index =
          ((decoded_bits >>
            static_cast<std::size_t>(BitshiftTraits::encoded_shift -
                                     (i * BitshiftTraits::nb_index_bits))) &
           mask)
              .to_ulong();
      out[i] = EncodingTraits::alphabet[index];
    }
  }

  template <typename OutputIterator>
  static void encode(
      std::bitset<BitshiftTraits::nb_decoded_bits> const& decoded_bits,
      OutputIterator out)
  {
    constexpr std::bitset<BitshiftTraits::nb_decoded_bits> mask(
        pow<2, BitshiftTraits::nb_index_bits>() - 1);

    for (auto i = 0; i < BitshiftTraits::nb_encoded_bytes; ++i)
    {
      auto const index =
          ((decoded_bits >>
            static_cast<std::size_t>(BitshiftTraits::encoded_shift -
                                     (i * BitshiftTraits::nb_index_bits))) &
           mask)
              .to_ulong();
      out[i] = EncodingTraits::alphabet[index];
    }
  }
};
}
}
