#include <catch2/catch.hpp>

#include <mgs/meta/concepts/complete_type.hpp>

using namespace mgs::meta;

TEST_CASE("complete_type")
{
  static_assert(is_complete_type<int>::value, "");
  static_assert(is_complete_type<struct incomplete*>::value, "");
  static_assert(!is_complete_type<struct incomplete>::value, "");
  static_assert(!is_complete_type<void>::value, "");
}
