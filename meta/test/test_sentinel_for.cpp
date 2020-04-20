#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/dereferenceable.hpp>
#include <mgs/meta/concepts/input_or_output_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

using namespace mgs::meta;

namespace
{
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

struct non_weakly_equality_comparable
{
};

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
}

TEST_CASE("sentinel_for")
{
  static_assert(is_sentinel_for<char*, char*>::value, "");
  static_assert(is_sentinel_for<pointer_sentinel, char*>::value, "");

  static_assert(!is_sentinel_for<non_weakly_equality_comparable, char*>::value, "");
  static_assert(!is_sentinel_for<pointer_sentinel, void*>::value, "");
  static_assert(!is_sentinel_for<non_semiregular, char*>::value, "");

  static_assert(!is_sentinel_for<void, void>::value, "");
  static_assert(!is_sentinel_for<struct incomplete, incomplete>::value, "");
}
