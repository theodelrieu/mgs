#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/swappable.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

using namespace mgs::meta;

namespace
{
struct non_default_constructible
{
  non_default_constructible() = delete;
  non_default_constructible(non_default_constructible const&) = default;
  non_default_constructible(non_default_constructible&&) = default;
  non_default_constructible& operator=(non_default_constructible const&) =
      default;
  non_default_constructible& operator=(non_default_constructible&&) = default;
  ~non_default_constructible() = default;
};

struct non_copy_constructible
{
  non_copy_constructible() = default;
  non_copy_constructible(non_copy_constructible const&) = delete;
  non_copy_constructible(non_copy_constructible&&) = default;
  non_copy_constructible& operator=(non_copy_constructible const&) = default;
  non_copy_constructible& operator=(non_copy_constructible&&) = default;
  ~non_copy_constructible() = default;
};

struct non_move_constructible
{
  non_move_constructible() = default;
  non_move_constructible(non_move_constructible const&) = default;
  non_move_constructible(non_move_constructible&&) = delete;
  non_move_constructible& operator=(non_move_constructible const&) = default;
  non_move_constructible& operator=(non_move_constructible&&) = default;
  ~non_move_constructible() = default;
};

struct non_copy_assignable
{
  non_copy_assignable() = default;
  non_copy_assignable(non_copy_assignable const&) = default;
  non_copy_assignable(non_copy_assignable&&) = default;
  non_copy_assignable& operator=(non_copy_assignable const&) = delete;
  non_copy_assignable& operator=(non_copy_assignable&&) = default;
  ~non_copy_assignable() = default;
};

struct non_move_assignable
{
  non_move_assignable() = default;
  non_move_assignable(non_move_assignable const&) = default;
  non_move_assignable(non_move_assignable&&) = default;
  non_move_assignable& operator=(non_move_assignable const&) = default;
  non_move_assignable& operator=(non_move_assignable&&) = delete;
  ~non_move_assignable() = default;
};

struct non_destructible
{
  non_destructible() = default;
  non_destructible(non_destructible const&) = default;
  non_destructible(non_destructible&&) = default;
  non_destructible& operator=(non_destructible const&) = default;
  non_destructible& operator=(non_destructible&&) = default;
  ~non_destructible() = delete;
};

struct non_swappable
{
};

void swap(non_default_constructible&, non_default_constructible&);
void swap(non_copy_constructible&, non_copy_constructible&);
void swap(non_move_constructible&, non_move_constructible&);
void swap(non_copy_assignable&, non_copy_assignable&);
void swap(non_move_assignable&, non_move_assignable&);
void swap(non_destructible&, non_destructible&);
void swap(non_swappable&, non_swappable&) = delete;

struct correct
{
};
}

TEST_CASE("semiregular")
{
  static_assert(is_semiregular<int>::value, "");
  static_assert(is_semiregular<correct>::value, "");

  static_assert(!is_semiregular<void>::value, "");
  static_assert(!is_semiregular<struct incomplete>::value, "");

  static_assert(!is_semiregular<non_default_constructible>::value, "");
  static_assert(!is_semiregular<non_copy_constructible>::value, "");
  static_assert(!is_semiregular<non_move_constructible>::value, "");
  static_assert(!is_semiregular<non_copy_assignable>::value, "");
  static_assert(!is_semiregular<non_move_assignable>::value, "");
  static_assert(!is_semiregular<non_destructible>::value, "");
  static_assert(!is_semiregular<non_swappable>::value, "");
}
