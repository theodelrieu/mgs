#include <istream>
#include <iterator>
#include <string>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/forward_iterator.hpp>
#include <mgs/meta/concepts/incrementable.hpp>

using namespace mgs::meta;

namespace
{
struct proxy
{
  int& operator*();
};

struct invalid_post_increment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  invalid_post_increment_iterator& operator++();
  proxy operator++(int);
  reference operator*();
};

bool operator==(invalid_post_increment_iterator,
                invalid_post_increment_iterator);
bool operator!=(invalid_post_increment_iterator,
                invalid_post_increment_iterator);

struct valid_forward_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  valid_forward_iterator& operator++();
  valid_forward_iterator operator++(int);
  reference operator*();
};

bool operator==(valid_forward_iterator, valid_forward_iterator);
bool operator!=(valid_forward_iterator, valid_forward_iterator);
}

TEST_CASE("forward_iterator")
{
  static_assert(is_forward_iterator<char*>::value, "");
  static_assert(is_forward_iterator<valid_forward_iterator>::value, "");

  static_assert(!is_forward_iterator<std::istreambuf_iterator<char>>::value,
                "");
  static_assert(!is_forward_iterator<invalid_post_increment_iterator>::value,
                "");

  static_assert(!is_forward_iterator<void>::value, "");
  static_assert(!is_forward_iterator<struct incomplete>::value, "");
  static_assert(!is_forward_iterator<void*>::value, "");
  static_assert(!is_forward_iterator<struct incomplete*>::value, "");
}
