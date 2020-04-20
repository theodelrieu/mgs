#pragma once

#include <algorithm>

#include <mgs/codecs/detail/input_reader.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S>
iterator_sentinel_source<I, S>::iterator_sentinel_source(I begin, S end)
  : _current(begin), _end(end)
{
}

template <typename I, typename S>
I iterator_sentinel_source<I, S>::begin() const
{
  return _current;
}

template <typename I, typename S>
S iterator_sentinel_source<I, S>::end() const
{
  return _end;
}

template <typename I, typename S>
template <typename OutputIterator, typename SFINAE>
std::pair<OutputIterator, mgs::ssize_t> iterator_sentinel_source<I, S>::read(
    OutputIterator out, mgs::ssize_t n)
{
  return detail::input_reader<I, S>::read(_current, _end, out, n);
}

template <typename I, typename S>
template <typename S2, typename SFINAE>
mgs::ssize_t iterator_sentinel_source<I, S>::max_remaining_size() const
{
  return _end - _current;
}
}
}
