#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename I, typename S, typename = void>
struct input_reader
{
  template <typename OutputIterator>
  static std::pair<OutputIterator, mgs::ssize_t> read(I& current,
                                                       S end,
                                                       OutputIterator out,
                                                       mgs::ssize_t n)
  {
    auto to_read = n;
    while (current != end && to_read > 0)
    {
      *out++ = *current++;
      --to_read;
    }
    return std::make_pair(std::move(out), n - to_read);
  }
};

template <typename I, typename S>
struct input_reader<I,
                    S,
                    std::enable_if_t<meta::is_sized_sentinel_for<S, I>::value>>
{
  template <typename OutputIterator>
  static std::pair<OutputIterator, mgs::ssize_t> read(I& current,
                                                       S end,
                                                       OutputIterator out,
                                                       mgs::ssize_t n)
  {
    auto const to_read = std::min<mgs::ssize_t>(end - current, n);
    out = std::copy_n(current, to_read, std::move(out));
    current += to_read;
    return std::make_pair(std::move(out), to_read);
  }
};
}
}
