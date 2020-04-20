#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>

using namespace mgs::meta;

namespace
{
struct valid_output_iterator
{
  using difference_type = std::ptrdiff_t;

  valid_output_iterator& operator++();
  valid_output_iterator& operator++(int);
  valid_output_iterator& operator*();

  template <typename T>
  valid_output_iterator& operator=(T&&);
};

struct non_assignable_iterator
{
  using difference_type = std::ptrdiff_t;

  non_assignable_iterator& operator++();
  non_assignable_iterator& operator++(int);
  non_assignable_iterator& operator*();
};
}

TEST_CASE("output_iterator")
{
  static_assert(is_output_iterator<char*, char>::value, "");
  static_assert(is_output_iterator<valid_output_iterator, int>::value, "");

  static_assert(!is_output_iterator<non_assignable_iterator, int>::value, "");
  static_assert(!is_output_iterator<void, int>::value, "");
  static_assert(!is_output_iterator<void*, int>::value, "");
  static_assert(!is_output_iterator<struct incomplete, int>::value, "");
  static_assert(!is_output_iterator<struct incomplete*, int>::value, "");
  static_assert(
      !is_output_iterator<struct incomplete*, struct incomplete&>::value, "");
}
