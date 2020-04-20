#pragma once

#include <bitset>
#include <cstddef>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename BitshiftTraits, typename RandomAccessIterator>
auto decoded_to_bitset(RandomAccessIterator it, mgs::ssize_t n)
{
  std::bitset<BitshiftTraits::nb_decoded_bits> input_bits;

  for (auto i = 0; i < n; ++i)
  {
    decltype(input_bits) bits(static_cast<unsigned long long>(it[i]));
    input_bits |= bits << static_cast<std::size_t>(
                      BitshiftTraits::nb_decoded_bits - 8 - (8 * i));
  }
  return input_bits;
}

template <typename EncodingTraits, typename RandomAccessIterator>
auto indices_to_bitset(RandomAccessIterator it, mgs::ssize_t n)
{
  using BitshiftTraits = bitshift_traits<EncodingTraits>;
  std::bitset<BitshiftTraits::nb_decoded_bits> input_bits;

  for (auto i = 0; i < n; ++i)
  {
    decltype(input_bits) bits(static_cast<unsigned long long>(
        EncodingTraits::index_of(static_cast<char>(it[i]))));
    input_bits |=
        bits << static_cast<std::size_t>(BitshiftTraits::nb_decoded_bits -
                                         BitshiftTraits::nb_index_bits -
                                         (BitshiftTraits::nb_index_bits * i));
  }
  return input_bits;
}
}
}
