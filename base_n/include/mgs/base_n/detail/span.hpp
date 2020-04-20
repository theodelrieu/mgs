#pragma once

#include <cstddef>

#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename U = T>
class span
{
public:
  span(T begin, U const end) : _begin(begin), _end(end)
  {
  }

  auto& operator[](mgs::ssize_t n)
  {
    return _begin[n];
  }

  auto begin()
  {
    return _begin;
  }

  auto begin() const
  {
    return _begin;
  }

  auto end()
  {
    return _end;
  }

  auto end() const
  {
    return _end;
  }

  mgs::ssize_t size() const
  {
    return _end - _begin;
  }

  auto const& operator[](mgs::ssize_t n) const
  {
    return _begin[n];
  }

private:
  T _begin;
  U _end;
};
}
}
