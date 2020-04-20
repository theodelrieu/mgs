#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
// a very minimalistic static_vector...
template <typename T, std::size_t N>
class static_vector
{
  static_assert(std::is_integral<T>::value, "");
  static_assert(sizeof(T) == 1, "");
  static_assert(N > 0 && N <= 256, "");

public:
  using value_type = T;

  void push_back(T c)
  {
    assert(_index + 1 <= static_cast<mgs::ssize_t>(N));
    _array[static_cast<std::size_t>(_index++)] = c;
  }

  mgs::ssize_t size() const
  {
    return _index;
  }

  void resize(mgs::ssize_t n)
  {
    assert(n >= 0 && n <= static_cast<mgs::ssize_t>(N));
    _index = n;
  }

  T& operator[](mgs::ssize_t n)
  {
    assert(n >= 0 && n <= _index);
    return _array[static_cast<std::size_t>(n)];
  }

  T const& operator[](mgs::ssize_t n) const
  {
    assert(n <= _index);
    return _array[static_cast<std::size_t>(n)];
  }

  T* begin()
  {
    return &_array[0];
  }

  T const* begin() const
  {
    return &_array[0];
  }

  T* data()
  {
    return begin();
  }

  T const* data() const
  {
    return begin();
  }

  T* end()
  {
    return &_array[0] + _index;
  }

  T const* end() const
  {
    return &_array[0] + _index;
  }

private:
  std::array<T, N> _array;
  mgs::ssize_t _index{0};
};
}
}
