#include <type_traits>
#include <bitset>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/boolean.hpp>

using namespace mgs::meta;

namespace
{
struct valid_boolean
{
  operator bool() const;
};

int operator==(valid_boolean, bool);
int operator==(bool, valid_boolean);
int operator==(valid_boolean, valid_boolean);

bool operator!(valid_boolean);

int operator!=(valid_boolean, bool);
int operator!=(bool, valid_boolean);
int operator!=(valid_boolean, valid_boolean);

bool operator&&(valid_boolean, bool);
bool operator&&(bool, valid_boolean);
bool operator&&(valid_boolean, valid_boolean);

bool operator||(valid_boolean, bool);
bool operator||(bool, valid_boolean);
bool operator||(valid_boolean, valid_boolean);

struct invalid_boolean
{
  operator bool() const;
};

int operator==(invalid_boolean, bool);
int operator==(bool, invalid_boolean);
int operator==(invalid_boolean, invalid_boolean);

int operator!(invalid_boolean);

int operator!=(invalid_boolean, bool);
int operator!=(bool, invalid_boolean);
int operator!=(invalid_boolean, invalid_boolean);

bool operator&&(invalid_boolean, bool);
int operator&&(bool, invalid_boolean);
bool operator&&(invalid_boolean, invalid_boolean);

bool operator||(invalid_boolean, bool);
bool operator||(bool, invalid_boolean);
bool operator||(invalid_boolean, invalid_boolean);
}

TEST_CASE("boolean")
{
  static_assert(is_boolean<char>::value, "");
  static_assert(is_boolean<bool>::value, "");
  static_assert(is_boolean<std::true_type>::value, "");
  static_assert(is_boolean<std::bitset<8>::reference>::value, "");
  static_assert(is_boolean<valid_boolean>::value, "");

  static_assert(!is_boolean<void>::value, "");
  static_assert(!is_boolean<invalid_boolean>::value, "");
  static_assert(!is_boolean<struct incomplete*>::value, "");
  static_assert(!is_boolean<char*>::value, "");
  static_assert(!is_boolean<std::unique_ptr<char>>::value, "");
  static_assert(!is_boolean<void()>::value, "");
  static_assert(!is_boolean<void (*)()>::value, "");
  static_assert(!is_boolean<void (&)()>::value, "");
}
