#pragma once

#include <cstddef>
#include <utility>

#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <mgs::ssize_t A, mgs::ssize_t Multiple>
constexpr mgs::ssize_t round_to_multiple_of()
{
  return ((A + (Multiple / 2)) / Multiple) * Multiple;
}

template <mgs::ssize_t N>
constexpr mgs::ssize_t log2()
{
  mgs::ssize_t ret = 0;
  auto n = N;

  while (n >>= 1)
    ++ret;
  return ret;
}

template <mgs::ssize_t N>
constexpr mgs::ssize_t is_power_of_2()
{
  return N != 0 && ((N & (N - 1)) == 0);
}

template <mgs::ssize_t X, mgs::ssize_t N>
constexpr mgs::ssize_t pow()
{
  mgs::ssize_t ret = 1;
  auto x = X;
  auto n = N;

  while (n > 0)
  {
    if (n & 1)
      ret *= x;
    n >>= 1;
    x *= x;
  }
  return ret;
}

template <mgs::ssize_t A, mgs::ssize_t B>
constexpr mgs::ssize_t gcd()
{
  auto a = A;
  auto b = B;

  while (b != 0)
  {
    auto const r = a % b;
    a = b;
    b = r;
  }
  return a;
}

template <mgs::ssize_t AlphabetSize>
constexpr std::pair<mgs::ssize_t, mgs::ssize_t> encoding_bytes()
{
  constexpr auto a = log2<AlphabetSize>();
  // log2(256) -- ASCII set
  constexpr auto b = 8;
  constexpr auto divisor = gcd<a, b>();

  return {a / divisor, b / divisor};
}

template <mgs::ssize_t AlphabetSize>
constexpr mgs::ssize_t decoded_bytes()
{
  return encoding_bytes<AlphabetSize>().first;
}

template <mgs::ssize_t AlphabetSize>
constexpr mgs::ssize_t encoded_bytes()
{
  return encoding_bytes<AlphabetSize>().second;
}
}
}
