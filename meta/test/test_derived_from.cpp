#include <catch2/catch.hpp>

#include <mgs/meta/concepts/derived_from.hpp>

using namespace mgs::meta;

namespace
{
struct base
{
};

struct derived : base
{
};
}

TEST_CASE("derived_from")
{
  static_assert(is_derived_from<derived, base>::value, "");
  static_assert(is_derived_from<base, base>::value, "");
  static_assert(is_derived_from<derived const volatile, base>::value, "");
  static_assert(is_derived_from<derived, base const volatile>::value, "");
  static_assert(!is_derived_from<base, derived>::value, "");

  static_assert(!is_derived_from<int, int>::value, "");
  static_assert(!is_derived_from<derived, void>::value, "");
  static_assert(!is_derived_from<void, base>::value, "");
}
