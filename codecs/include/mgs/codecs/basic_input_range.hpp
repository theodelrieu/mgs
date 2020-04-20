#pragma once

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detail/basic_input_range_iterator.hpp>

namespace mgs
{
namespace codecs
{
template <typename InputSource>
class basic_input_range
{
  static_assert(
      is_input_source<InputSource>::value,
      "basic_input_range's parameter must model codecs::input_source");

  using underlying_input_source = InputSource;

  friend detail::basic_input_range_iterator<basic_input_range>;
  using iterator = detail::basic_input_range_iterator<basic_input_range>;

public:
  explicit basic_input_range(InputSource);

  iterator begin();
  iterator end();

private:
  InputSource _input_source;
};

template <typename InputSource,
          typename = std::enable_if_t<is_input_source<InputSource>::value>>
basic_input_range<InputSource> make_input_range(InputSource);
}
}

#include <mgs/codecs/detail/basic_input_range_impl.hpp>
