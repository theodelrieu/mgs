#pragma once

#include <memory>

namespace mgs
{
namespace detail
{
template <typename BasicInputRange>
basic_input_range_iterator<BasicInputRange>::basic_input_range_iterator(
    BasicInputRange* e)
  : _range(e)
{
  if (_range)
  {
    auto res = _range->_input_source.read(&_last_read, 1);
    if (res.second == 0)
      _range = nullptr;
  }
}

template <typename BasicInputRange>
auto basic_input_range_iterator<BasicInputRange>::operator*() const -> reference
{
  return _last_read;
}

template <typename BasicInputRange>
auto basic_input_range_iterator<BasicInputRange>::operator-> () const -> pointer
{
  return std::addressof(**this);
}

template <typename BasicInputRange>
auto basic_input_range_iterator<BasicInputRange>::operator++()
    -> basic_input_range_iterator<BasicInputRange>&
{
  auto res = _range->_input_source.read(&_last_read, 1);
  if (res.second == 0)
    _range = nullptr;
  return *this;
}

template <typename BasicInputRange>
auto basic_input_range_iterator<BasicInputRange>::operator++(int)
    -> basic_input_range_iterator<BasicInputRange>
{
  auto ret = *this;
  ++(*this);
  return ret;
}

template <typename BasicInputRange>
bool basic_input_range_iterator<BasicInputRange>::equals(
    basic_input_range_iterator<BasicInputRange> const& rhs) const
{
  if (this->_range == nullptr || rhs._range == nullptr)
    return this->_range == rhs._range;
  return true;
}

template <typename BasicInputRange>
bool operator==(basic_input_range_iterator<BasicInputRange> const& lhs,
                basic_input_range_iterator<BasicInputRange> const& rhs) noexcept
{
  return lhs.equals(rhs);
}

template <typename BasicInputRange>
bool operator!=(basic_input_range_iterator<BasicInputRange> const& lhs,
                basic_input_range_iterator<BasicInputRange> const& rhs) noexcept
{
  return !(lhs == rhs);
}
}
}
