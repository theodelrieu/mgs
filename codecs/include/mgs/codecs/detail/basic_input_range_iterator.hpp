#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include <mgs/meta/concepts/derived_from.hpp>
#include <mgs/meta/range_reference_t.hpp>
#include <mgs/meta/range_value_t.hpp>

namespace mgs
{
namespace detail
{
template <typename BasicInputRange>
class basic_input_range_iterator
{
private:
  bool equals(basic_input_range_iterator const&) const;

public:
  using value_type =
      typename BasicInputRange::underlying_input_source::element_type;
  using difference_type = std::streamoff;

  using reference = value_type const&;
  using const_reference = reference;

  using pointer = value_type const*;
  using const_pointer = pointer;

  using iterator_category = std::input_iterator_tag;
  using iterator_concept = std::input_iterator_tag;

  basic_input_range_iterator() = default;
  explicit basic_input_range_iterator(BasicInputRange*);

  reference operator*() const;
  pointer operator->() const;

  basic_input_range_iterator& operator++();
  basic_input_range_iterator operator++(int);

  template <typename T>
  friend bool operator==(basic_input_range_iterator<T> const&,
                         basic_input_range_iterator<T> const&) noexcept;

private:
  BasicInputRange* _range{nullptr};
  value_type _last_read{};
};

template <typename T>
bool operator!=(basic_input_range_iterator<T> const&,
                basic_input_range_iterator<T> const&) noexcept;
}
}

#include <mgs/codecs/detail/basic_input_range_iterator_impl.hpp>
