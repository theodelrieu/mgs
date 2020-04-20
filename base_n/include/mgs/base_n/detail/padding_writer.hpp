#pragma once

#include <mgs/base_n/padding_policy.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          base_n::padding_policy Policy =
              EncodingTraits::padding_policy>
struct padding_writer
{
  template <typename OutputIterator>
  static void write(OutputIterator, mgs::ssize_t)
  {
  }
};

template <typename EncodingTraits>
struct padding_writer<EncodingTraits, base_n::padding_policy::required>
{
  template <typename OutputIterator>
  static void write(OutputIterator out, mgs::ssize_t n)
  {
    while (n-- > 0)
      *out++ = EncodingTraits::padding_character;
  }
};
}
}
