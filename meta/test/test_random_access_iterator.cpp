#include <iterator>
#include <list>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/random_access_iterator.hpp>

using namespace mgs::meta;

namespace
{
struct no_array_subscript_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_array_subscript_iterator& operator++();
  no_array_subscript_iterator operator++(int);
  no_array_subscript_iterator& operator--();
  no_array_subscript_iterator operator--(int);

  no_array_subscript_iterator& operator+=(difference_type);
  no_array_subscript_iterator& operator-=(difference_type);
  no_array_subscript_iterator operator+(difference_type) const;
  no_array_subscript_iterator operator-(difference_type) const;
  friend no_array_subscript_iterator operator+(difference_type,
                                               no_array_subscript_iterator);
  friend difference_type operator-(no_array_subscript_iterator,
                                   no_array_subscript_iterator);
  reference operator*();
};

bool operator==(no_array_subscript_iterator, no_array_subscript_iterator);
bool operator!=(no_array_subscript_iterator, no_array_subscript_iterator);
bool operator<(no_array_subscript_iterator, no_array_subscript_iterator);
bool operator<=(no_array_subscript_iterator, no_array_subscript_iterator);
bool operator>(no_array_subscript_iterator, no_array_subscript_iterator);
bool operator>=(no_array_subscript_iterator, no_array_subscript_iterator);

struct no_addition_assignment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_addition_assignment_iterator& operator++();
  no_addition_assignment_iterator operator++(int);
  no_addition_assignment_iterator& operator--();
  no_addition_assignment_iterator operator--(int);

  no_addition_assignment_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  no_addition_assignment_iterator operator+(difference_type) const;
  no_addition_assignment_iterator operator-(difference_type) const;
  friend no_addition_assignment_iterator operator+(
      difference_type, no_addition_assignment_iterator);
  friend difference_type operator-(no_addition_assignment_iterator,
                                   no_addition_assignment_iterator);
  reference operator*();
};

bool operator==(no_addition_assignment_iterator,
                no_addition_assignment_iterator);
bool operator!=(no_addition_assignment_iterator,
                no_addition_assignment_iterator);
bool operator<(no_addition_assignment_iterator,
               no_addition_assignment_iterator);
bool operator<=(no_addition_assignment_iterator,
                no_addition_assignment_iterator);
bool operator>(no_addition_assignment_iterator,
               no_addition_assignment_iterator);
bool operator>=(no_addition_assignment_iterator,
                no_addition_assignment_iterator);

struct no_member_addition_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_member_addition_iterator& operator++();
  no_member_addition_iterator operator++(int);
  no_member_addition_iterator& operator--();
  no_member_addition_iterator operator--(int);

  no_member_addition_iterator& operator+=(difference_type);
  no_member_addition_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  no_member_addition_iterator operator-(difference_type) const;
  friend no_member_addition_iterator operator+(difference_type,
                                               no_member_addition_iterator);
  friend difference_type operator-(no_member_addition_iterator,
                                   no_member_addition_iterator);
  reference operator*();
};

bool operator==(no_member_addition_iterator, no_member_addition_iterator);
bool operator!=(no_member_addition_iterator, no_member_addition_iterator);
bool operator<(no_member_addition_iterator, no_member_addition_iterator);
bool operator<=(no_member_addition_iterator, no_member_addition_iterator);
bool operator>(no_member_addition_iterator, no_member_addition_iterator);
bool operator>=(no_member_addition_iterator, no_member_addition_iterator);

struct no_free_addition_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_free_addition_iterator& operator++();
  no_free_addition_iterator operator++(int);
  no_free_addition_iterator& operator--();
  no_free_addition_iterator operator--(int);

  no_free_addition_iterator& operator+=(difference_type);
  no_free_addition_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  no_free_addition_iterator operator+(difference_type) const;
  no_free_addition_iterator operator-(difference_type) const;
  friend difference_type operator-(no_free_addition_iterator,
                                   no_free_addition_iterator);
  reference operator*();
};

bool operator==(no_free_addition_iterator, no_free_addition_iterator);
bool operator!=(no_free_addition_iterator, no_free_addition_iterator);
bool operator<(no_free_addition_iterator, no_free_addition_iterator);
bool operator<=(no_free_addition_iterator, no_free_addition_iterator);
bool operator>(no_free_addition_iterator, no_free_addition_iterator);
bool operator>=(no_free_addition_iterator, no_free_addition_iterator);

struct no_substraction_assignment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_substraction_assignment_iterator& operator++();
  no_substraction_assignment_iterator operator++(int);
  no_substraction_assignment_iterator& operator--();
  no_substraction_assignment_iterator operator--(int);

  no_substraction_assignment_iterator& operator+=(difference_type);
  reference operator[](difference_type) const;
  no_substraction_assignment_iterator operator+(difference_type) const;
  no_substraction_assignment_iterator operator-(difference_type) const;
  friend no_substraction_assignment_iterator operator+(
      difference_type, no_substraction_assignment_iterator);
  friend difference_type operator-(no_substraction_assignment_iterator,
                                   no_substraction_assignment_iterator);
  reference operator*();
};

bool operator==(no_substraction_assignment_iterator,
                no_substraction_assignment_iterator);
bool operator!=(no_substraction_assignment_iterator,
                no_substraction_assignment_iterator);
bool operator<(no_substraction_assignment_iterator,
               no_substraction_assignment_iterator);
bool operator<=(no_substraction_assignment_iterator,
                no_substraction_assignment_iterator);
bool operator>(no_substraction_assignment_iterator,
               no_substraction_assignment_iterator);
bool operator>=(no_substraction_assignment_iterator,
                no_substraction_assignment_iterator);

struct no_member_substraction_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_member_substraction_iterator& operator++();
  no_member_substraction_iterator operator++(int);
  no_member_substraction_iterator& operator--();
  no_member_substraction_iterator operator--(int);

  no_member_substraction_iterator& operator+=(difference_type);
  no_member_substraction_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  no_member_substraction_iterator operator+(difference_type) const;
  friend no_member_substraction_iterator operator+(
      difference_type, no_member_substraction_iterator);
  friend difference_type operator-(no_member_substraction_iterator,
                                   no_member_substraction_iterator);
  reference operator*();
};

bool operator==(no_member_substraction_iterator,
                no_member_substraction_iterator);
bool operator!=(no_member_substraction_iterator,
                no_member_substraction_iterator);
bool operator<(no_member_substraction_iterator,
               no_member_substraction_iterator);
bool operator<=(no_member_substraction_iterator,
                no_member_substraction_iterator);
bool operator>(no_member_substraction_iterator,
               no_member_substraction_iterator);
bool operator>=(no_member_substraction_iterator,
                no_member_substraction_iterator);

struct no_free_substraction_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  no_free_substraction_iterator& operator++();
  no_free_substraction_iterator operator++(int);
  no_free_substraction_iterator& operator--();
  no_free_substraction_iterator operator--(int);

  no_free_substraction_iterator& operator+=(difference_type);
  no_free_substraction_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  no_free_substraction_iterator operator+(difference_type) const;
  no_free_substraction_iterator operator-(difference_type) const;
  friend no_free_substraction_iterator operator+(difference_type,
                                                 no_free_substraction_iterator);
  reference operator*();
};

bool operator==(no_free_substraction_iterator, no_free_substraction_iterator);
bool operator!=(no_free_substraction_iterator, no_free_substraction_iterator);
bool operator<(no_free_substraction_iterator, no_free_substraction_iterator);
bool operator<=(no_free_substraction_iterator, no_free_substraction_iterator);
bool operator>(no_free_substraction_iterator, no_free_substraction_iterator);
bool operator>=(no_free_substraction_iterator, no_free_substraction_iterator);

struct invalid_array_subscript_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_array_subscript_iterator& operator++();
  invalid_array_subscript_iterator operator++(int);
  invalid_array_subscript_iterator& operator--();
  invalid_array_subscript_iterator operator--(int);

  invalid_array_subscript_iterator& operator+=(difference_type);
  invalid_array_subscript_iterator& operator-=(difference_type);
  invalid_array_subscript_iterator operator+(difference_type) const;
  invalid_array_subscript_iterator operator-(difference_type) const;
  void operator[](difference_type) const;
  friend invalid_array_subscript_iterator operator+(
      difference_type, invalid_array_subscript_iterator);
  friend difference_type operator-(invalid_array_subscript_iterator,
                                   invalid_array_subscript_iterator);
  reference operator*();
};

bool operator==(invalid_array_subscript_iterator,
                invalid_array_subscript_iterator);
bool operator!=(invalid_array_subscript_iterator,
                invalid_array_subscript_iterator);
bool operator<(invalid_array_subscript_iterator,
               invalid_array_subscript_iterator);
bool operator<=(invalid_array_subscript_iterator,
                invalid_array_subscript_iterator);
bool operator>(invalid_array_subscript_iterator,
               invalid_array_subscript_iterator);
bool operator>=(invalid_array_subscript_iterator,
                invalid_array_subscript_iterator);

struct invalid_addition_assignment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_addition_assignment_iterator& operator++();
  invalid_addition_assignment_iterator operator++(int);
  invalid_addition_assignment_iterator& operator--();
  invalid_addition_assignment_iterator operator--(int);

  invalid_addition_assignment_iterator& operator-=(difference_type);
  void operator+=(difference_type);
  reference operator[](difference_type) const;
  invalid_addition_assignment_iterator operator+(difference_type) const;
  invalid_addition_assignment_iterator operator-(difference_type) const;
  friend invalid_addition_assignment_iterator operator+(
      difference_type, invalid_addition_assignment_iterator);
  friend difference_type operator-(invalid_addition_assignment_iterator,
                                   invalid_addition_assignment_iterator);
  reference operator*();
};

bool operator==(invalid_addition_assignment_iterator,
                invalid_addition_assignment_iterator);
bool operator!=(invalid_addition_assignment_iterator,
                invalid_addition_assignment_iterator);
bool operator<(invalid_addition_assignment_iterator,
               invalid_addition_assignment_iterator);
bool operator<=(invalid_addition_assignment_iterator,
                invalid_addition_assignment_iterator);
bool operator>(invalid_addition_assignment_iterator,
               invalid_addition_assignment_iterator);
bool operator>=(invalid_addition_assignment_iterator,
                invalid_addition_assignment_iterator);

struct invalid_member_addition_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_member_addition_iterator& operator++();
  invalid_member_addition_iterator operator++(int);
  invalid_member_addition_iterator& operator--();
  invalid_member_addition_iterator operator--(int);

  invalid_member_addition_iterator& operator+=(difference_type);
  invalid_member_addition_iterator& operator-=(difference_type);
  void operator+(difference_type) const;
  reference operator[](difference_type) const;
  invalid_member_addition_iterator operator-(difference_type) const;
  friend invalid_member_addition_iterator operator+(
      difference_type, invalid_member_addition_iterator);
  friend difference_type operator-(invalid_member_addition_iterator,
                                   invalid_member_addition_iterator);
  reference operator*();
};

bool operator==(invalid_member_addition_iterator,
                invalid_member_addition_iterator);
bool operator!=(invalid_member_addition_iterator,
                invalid_member_addition_iterator);
bool operator<(invalid_member_addition_iterator,
               invalid_member_addition_iterator);
bool operator<=(invalid_member_addition_iterator,
                invalid_member_addition_iterator);
bool operator>(invalid_member_addition_iterator,
               invalid_member_addition_iterator);
bool operator>=(invalid_member_addition_iterator,
                invalid_member_addition_iterator);

struct invalid_free_addition_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_free_addition_iterator& operator++();
  invalid_free_addition_iterator operator++(int);
  invalid_free_addition_iterator& operator--();
  invalid_free_addition_iterator operator--(int);

  invalid_free_addition_iterator& operator+=(difference_type);
  invalid_free_addition_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  invalid_free_addition_iterator operator+(difference_type) const;
  friend void operator+(difference_type, invalid_free_addition_iterator);
  invalid_free_addition_iterator operator-(difference_type) const;
  friend difference_type operator-(invalid_free_addition_iterator,
                                   invalid_free_addition_iterator);
  reference operator*();
};

bool operator==(invalid_free_addition_iterator, invalid_free_addition_iterator);
bool operator!=(invalid_free_addition_iterator, invalid_free_addition_iterator);
bool operator<(invalid_free_addition_iterator, invalid_free_addition_iterator);
bool operator<=(invalid_free_addition_iterator, invalid_free_addition_iterator);
bool operator>(invalid_free_addition_iterator, invalid_free_addition_iterator);
bool operator>=(invalid_free_addition_iterator, invalid_free_addition_iterator);

struct invalid_substraction_assignment_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_substraction_assignment_iterator& operator++();
  invalid_substraction_assignment_iterator operator++(int);
  invalid_substraction_assignment_iterator& operator--();
  invalid_substraction_assignment_iterator operator--(int);

  invalid_substraction_assignment_iterator& operator+=(difference_type);
  void operator-=(difference_type);
  reference operator[](difference_type) const;
  invalid_substraction_assignment_iterator operator+(difference_type) const;
  invalid_substraction_assignment_iterator operator-(difference_type) const;
  friend invalid_substraction_assignment_iterator operator+(
      difference_type, invalid_substraction_assignment_iterator);
  friend difference_type operator-(invalid_substraction_assignment_iterator,
                                   invalid_substraction_assignment_iterator);
  reference operator*();
};

bool operator==(invalid_substraction_assignment_iterator,
                invalid_substraction_assignment_iterator);
bool operator!=(invalid_substraction_assignment_iterator,
                invalid_substraction_assignment_iterator);
bool operator<(invalid_substraction_assignment_iterator,
               invalid_substraction_assignment_iterator);
bool operator<=(invalid_substraction_assignment_iterator,
                invalid_substraction_assignment_iterator);
bool operator>(invalid_substraction_assignment_iterator,
               invalid_substraction_assignment_iterator);
bool operator>=(invalid_substraction_assignment_iterator,
                invalid_substraction_assignment_iterator);

struct invalid_member_substraction_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_member_substraction_iterator& operator++();
  invalid_member_substraction_iterator operator++(int);
  invalid_member_substraction_iterator& operator--();
  invalid_member_substraction_iterator operator--(int);

  invalid_member_substraction_iterator& operator+=(difference_type);
  invalid_member_substraction_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  void operator-(difference_type) const;
  invalid_member_substraction_iterator operator+(difference_type) const;
  friend invalid_member_substraction_iterator operator+(
      difference_type, invalid_member_substraction_iterator);
  friend difference_type operator-(invalid_member_substraction_iterator,
                                   invalid_member_substraction_iterator);
  reference operator*();
};

bool operator==(invalid_member_substraction_iterator,
                invalid_member_substraction_iterator);
bool operator!=(invalid_member_substraction_iterator,
                invalid_member_substraction_iterator);
bool operator<(invalid_member_substraction_iterator,
               invalid_member_substraction_iterator);
bool operator<=(invalid_member_substraction_iterator,
                invalid_member_substraction_iterator);
bool operator>(invalid_member_substraction_iterator,
               invalid_member_substraction_iterator);
bool operator>=(invalid_member_substraction_iterator,
                invalid_member_substraction_iterator);

struct invalid_free_substraction_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  invalid_free_substraction_iterator& operator++();
  invalid_free_substraction_iterator operator++(int);
  invalid_free_substraction_iterator& operator--();
  invalid_free_substraction_iterator operator--(int);

  invalid_free_substraction_iterator& operator+=(difference_type);
  invalid_free_substraction_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  invalid_free_substraction_iterator operator+(difference_type) const;
  invalid_free_substraction_iterator operator-(difference_type) const;
  friend invalid_free_substraction_iterator operator+(
      difference_type, invalid_free_substraction_iterator);
  friend void operator-(invalid_free_substraction_iterator,
                        invalid_free_substraction_iterator);
  reference operator*();
};

bool operator==(invalid_free_substraction_iterator,
                invalid_free_substraction_iterator);
bool operator!=(invalid_free_substraction_iterator,
                invalid_free_substraction_iterator);
bool operator<(invalid_free_substraction_iterator,
               invalid_free_substraction_iterator);
bool operator<=(invalid_free_substraction_iterator,
                invalid_free_substraction_iterator);
bool operator>(invalid_free_substraction_iterator,
               invalid_free_substraction_iterator);
bool operator>=(invalid_free_substraction_iterator,
                invalid_free_substraction_iterator);

struct non_totally_ordered_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  non_totally_ordered_iterator& operator++();
  non_totally_ordered_iterator operator++(int);
  non_totally_ordered_iterator& operator--();
  non_totally_ordered_iterator operator--(int);

  non_totally_ordered_iterator& operator+=(difference_type);
  non_totally_ordered_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  non_totally_ordered_iterator operator+(difference_type) const;
  non_totally_ordered_iterator operator-(difference_type) const;
  friend non_totally_ordered_iterator operator+(
      difference_type, non_totally_ordered_iterator);
  friend difference_type operator-(non_totally_ordered_iterator,
                                   non_totally_ordered_iterator);
  reference operator*();
};

bool operator==(non_totally_ordered_iterator,
                non_totally_ordered_iterator);
bool operator!=(non_totally_ordered_iterator,
                non_totally_ordered_iterator);

struct valid_random_access_iterator
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  valid_random_access_iterator& operator++();
  valid_random_access_iterator operator++(int);
  valid_random_access_iterator& operator--();
  valid_random_access_iterator operator--(int);

  valid_random_access_iterator& operator+=(difference_type);
  valid_random_access_iterator& operator-=(difference_type);
  reference operator[](difference_type) const;
  valid_random_access_iterator operator+(difference_type) const;
  valid_random_access_iterator operator-(difference_type) const;
  friend valid_random_access_iterator operator+(difference_type,
                                                valid_random_access_iterator);
  friend difference_type operator-(valid_random_access_iterator,
                                   valid_random_access_iterator);
  reference operator*();
};

bool operator==(valid_random_access_iterator, valid_random_access_iterator);
bool operator!=(valid_random_access_iterator, valid_random_access_iterator);
bool operator<(valid_random_access_iterator, valid_random_access_iterator);
bool operator<=(valid_random_access_iterator, valid_random_access_iterator);
bool operator>(valid_random_access_iterator, valid_random_access_iterator);
bool operator>=(valid_random_access_iterator, valid_random_access_iterator);
}

TEST_CASE("random_access_iterator")
{
  static_assert(is_random_access_iterator<valid_random_access_iterator>::value,
                "");

  static_assert(!is_random_access_iterator<std::list<char>>::value, "");
  static_assert(!is_random_access_iterator<no_array_subscript_iterator>::value,
                "");
  static_assert(
      !is_random_access_iterator<no_addition_assignment_iterator>::value, "");
  static_assert(!is_random_access_iterator<no_member_addition_iterator>::value,
                "");
  static_assert(
      !is_random_access_iterator<no_substraction_assignment_iterator>::value,
      "");
  static_assert(
      !is_random_access_iterator<no_member_substraction_iterator>::value, "");
  static_assert(
      !is_random_access_iterator<invalid_array_subscript_iterator>::value, "");
  static_assert(
      !is_random_access_iterator<invalid_addition_assignment_iterator>::value,
      "");
  static_assert(
      !is_random_access_iterator<invalid_member_addition_iterator>::value, "");
  static_assert(!is_random_access_iterator<
                    invalid_substraction_assignment_iterator>::value,
                "");
  static_assert(
      !is_random_access_iterator<invalid_member_substraction_iterator>::value,
      "");
  static_assert(
      !is_random_access_iterator<non_totally_ordered_iterator>::value,
      "");

  static_assert(!is_random_access_iterator<void>::value, "");
  static_assert(!is_random_access_iterator<struct incomplete>::value, "");
}
