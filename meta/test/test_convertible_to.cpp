#include <catch2/catch.hpp>

#include <mgs/meta/concepts/convertible_to.hpp>

using namespace mgs::meta;

namespace
{
struct A
{
};

struct B
{
  explicit operator A();
};

struct C
{
  operator A();
};
}

TEST_CASE("convertible_to")
{
  static_assert(is_convertible_to<void, void>::value, "");
  static_assert(!is_convertible_to<struct incomplete, void>::value, "");
  static_assert(!is_convertible_to<void, struct incomplete>::value, "");

  static_assert(is_convertible_to<int(&&)[], int*>::value, "");
  static_assert(is_convertible_to<int[], int*>::value, "");
  static_assert(is_convertible_to<C, A>::value, "");
  static_assert(!is_convertible_to<B, A>::value, "");
}
