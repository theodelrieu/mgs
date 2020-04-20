#include <istream>
#include <iterator>
#include <string>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/common_reference_with.hpp>
#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>

using namespace mgs::meta;

namespace
{
struct output_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = void;
  using iterator_category = std::output_iterator_tag;

  output_iterator& operator++();
  void operator++(int);
  void operator*();
};

struct invalid_reference_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = std::string;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  invalid_reference_iterator& operator++();
  invalid_reference_iterator operator++(int);
  reference operator*();
};

struct valid_input_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  valid_input_iterator& operator++();
  valid_input_iterator operator++(int);
  reference operator*();
};
}

TEST_CASE("input_iterator")
{
  static_assert(is_input_iterator<char*>::value, "");
  static_assert(is_input_iterator<std::istreambuf_iterator<char>>::value, "");
  static_assert(is_input_iterator<valid_input_iterator>::value, "");

  static_assert(
      !is_input_iterator<std::back_insert_iterator<std::string>>::value, "");
  static_assert(!is_input_iterator<output_iterator>::value, "");
  static_assert(!is_input_iterator<invalid_reference_iterator>::value, "");
  static_assert(!is_input_iterator<void>::value, "");
  static_assert(!is_input_iterator<struct incomplete>::value, "");
}
