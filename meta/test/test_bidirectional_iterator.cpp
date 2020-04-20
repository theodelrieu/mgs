#include <forward_list>
#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/bidirectional_iterator.hpp>
#include <mgs/meta/concepts/incrementable.hpp>

using namespace mgs::meta;

namespace
{
struct no_post_decrement_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  no_post_decrement_iterator& operator++();
  no_post_decrement_iterator operator++(int);
  no_post_decrement_iterator& operator--();
  reference operator*();
};

bool operator==(no_post_decrement_iterator, no_post_decrement_iterator);
bool operator!=(no_post_decrement_iterator, no_post_decrement_iterator);

struct no_pre_decrement_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  no_pre_decrement_iterator& operator++();
  no_pre_decrement_iterator operator++(int);
  no_pre_decrement_iterator operator--(int);
  reference operator*();
};

bool operator==(no_pre_decrement_iterator, no_pre_decrement_iterator);
bool operator!=(no_pre_decrement_iterator, no_pre_decrement_iterator);

struct invalid_post_decrement_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  invalid_post_decrement_iterator& operator++();
  invalid_post_decrement_iterator operator++(int);
  invalid_post_decrement_iterator& operator--();
  void operator--(int);
  reference operator*();
};

bool operator==(invalid_post_decrement_iterator,
                invalid_post_decrement_iterator);
bool operator!=(invalid_post_decrement_iterator,
                invalid_post_decrement_iterator);

struct valid_bidirectional_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  valid_bidirectional_iterator& operator++();
  valid_bidirectional_iterator operator++(int);
  valid_bidirectional_iterator& operator--();
  valid_bidirectional_iterator operator--(int);
  reference operator*();
};

bool operator==(valid_bidirectional_iterator, valid_bidirectional_iterator);
bool operator!=(valid_bidirectional_iterator, valid_bidirectional_iterator);
}

TEST_CASE("bidirectional_iterator")
{
  static_assert(is_bidirectional_iterator<char*>::value, "");
  static_assert(is_bidirectional_iterator<valid_bidirectional_iterator>::value,
                "");

  static_assert(!is_bidirectional_iterator<std::forward_list<char>>::value, "");
  static_assert(!is_bidirectional_iterator<no_post_decrement_iterator>::value,
                "");
  static_assert(!is_bidirectional_iterator<no_pre_decrement_iterator>::value,
                "");
  static_assert(
      !is_bidirectional_iterator<invalid_post_decrement_iterator>::value, "");

  static_assert(!is_bidirectional_iterator<void>::value, "");
  static_assert(!is_bidirectional_iterator<struct incomplete>::value, "");
}
