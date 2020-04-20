#include <iterator>
#include <tuple>
#include <iostream>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/bidirectional_iterator.hpp>

#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>

using namespace mgs::meta;

namespace
{
struct non_semiregular
{
  non_semiregular() = delete;
};

template <typename T>
bool operator==(T*, non_semiregular);
template <typename T>
bool operator!=(T*, non_semiregular);
template <typename T>
bool operator==(non_semiregular, T*);
template <typename T>
bool operator!=(non_semiregular, T*);

struct pointer_sentinel
{
};

template <typename T>
bool operator==(T*, pointer_sentinel);
template <typename T>
bool operator!=(T*, pointer_sentinel);
template <typename T>
bool operator==(pointer_sentinel, T*);
template <typename T>
bool operator!=(pointer_sentinel, T*);

struct valid_sized_sentinel : pointer_sentinel
{
};

template <typename T>
std::ptrdiff_t operator-(valid_sized_sentinel, T*);

template <typename T>
std::ptrdiff_t operator-(T*, valid_sized_sentinel);

struct invalid_difference_type_sized_sentinel : valid_sized_sentinel
{
};

template <typename T>
void operator-(invalid_difference_type_sized_sentinel, T*);

template <typename T>
void operator-(T*, invalid_difference_type_sized_sentinel);
}

TEST_CASE("sized_sentinel_for")
{
  static_assert(is_sized_sentinel_for<char*, char*>::value, "");
  static_assert(is_sized_sentinel_for<valid_sized_sentinel, char*>::value, "");

  static_assert(!is_sized_sentinel_for<pointer_sentinel, void*>::value, "");
  static_assert(
      !is_sized_sentinel_for<invalid_difference_type_sized_sentinel, void*>::value,
      "");
  static_assert(!is_sized_sentinel_for<non_semiregular, char*>::value, "");

  static_assert(!is_sized_sentinel_for<void, void>::value, "");
  static_assert(!is_sized_sentinel_for<struct incomplete, incomplete>::value, "");
}
