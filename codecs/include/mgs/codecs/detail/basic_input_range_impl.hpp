#pragma once

#include <utility>

namespace mgs
{
namespace codecs
{
template <typename InputSource>
basic_input_range<InputSource>::basic_input_range(InputSource input_source)
  : _input_source(std::move(input_source))
{
}

template <typename InputSource>
auto basic_input_range<InputSource>::begin() -> iterator
{
  return iterator{this};
}

template <typename InputSource>
auto basic_input_range<InputSource>::end() -> iterator
{
  return iterator{nullptr};
}

template <typename InputSource, typename SFINAE>
basic_input_range<InputSource> make_input_range(InputSource is)
{
  return basic_input_range<InputSource>{std::move(is)};
}
}
}
