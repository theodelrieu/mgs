#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/incrementable.hpp>
#include <mgs/meta/concepts/weakly_incrementable.hpp>
#include <mgs/meta/concepts/regular.hpp>

using namespace mgs::meta;

namespace
{
struct non_post_incrementable
{
  using difference_type = std::ptrdiff_t;
  non_post_incrementable& operator++();
};

struct non_pre_incrementable
{
  using difference_type = std::ptrdiff_t;
  void operator++(int);
};

struct valid_weakly_incrementable
{
  using difference_type = std::ptrdiff_t;

  valid_weakly_incrementable& operator++();
  valid_weakly_incrementable operator++(int);
};

struct regular_and_weakly_incrementable
{
  using difference_type = std::ptrdiff_t;

  regular_and_weakly_incrementable& operator++();
  void operator++(int);
};

bool operator==(regular_and_weakly_incrementable,
                regular_and_weakly_incrementable);
bool operator!=(regular_and_weakly_incrementable,
                regular_and_weakly_incrementable);

struct non_regular
{
  using difference_type = std::ptrdiff_t;
  non_regular& operator++();
  non_regular operator++(int);
};

struct valid_incrementable
{
  using difference_type = std::ptrdiff_t;
  valid_incrementable& operator++();
  valid_incrementable operator++(int);
};

bool operator==(valid_incrementable, valid_incrementable);
bool operator!=(valid_incrementable, valid_incrementable);
}

TEST_CASE("weakly_incrementable")
{
  static_assert(is_weakly_incrementable<char*>::value, "");
  static_assert(is_weakly_incrementable<char>::value, "");
  static_assert(is_weakly_incrementable<valid_weakly_incrementable>::value, "");
  static_assert(
      is_weakly_incrementable<regular_and_weakly_incrementable>::value, "");
  static_assert(
      is_weakly_incrementable<std::back_insert_iterator<std::string>>::value,
      "");

  static_assert(!is_weakly_incrementable<void>::value, "");
  static_assert(!is_weakly_incrementable<struct incomplete>::value, "");
  static_assert(!is_weakly_incrementable<non_post_incrementable>::value, "");
  static_assert(!is_weakly_incrementable<non_pre_incrementable>::value, "");
}

TEST_CASE("incrementable")
{
  static_assert(is_incrementable<char*>::value, "");
  static_assert(is_incrementable<char>::value, "");
  static_assert(is_incrementable<valid_incrementable>::value, "");

  static_assert(!is_incrementable<non_regular>::value, "");
  static_assert(!is_incrementable<regular_and_weakly_incrementable>::value, "");
  static_assert(!is_incrementable<void>::value, "");
  static_assert(!is_incrementable<struct incomplete>::value, "");
  static_assert(!is_incrementable<non_post_incrementable>::value, "");
  static_assert(!is_incrementable<non_pre_incrementable>::value, "");
  static_assert(!is_incrementable<void*>::value, "");
  static_assert(!is_incrementable<int (*)()>::value, "");
}
