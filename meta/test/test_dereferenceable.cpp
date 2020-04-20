#include <memory>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/dereferenceable.hpp>

using namespace mgs::meta;

namespace
{
struct invalid
{
  void operator*();
};
}

TEST_CASE("dereferenceable")
{
  static_assert(!is_dereferenceable<char>::value, "");
  static_assert(!is_dereferenceable<void>::value, "");
  static_assert(!is_dereferenceable<void*>::value, "");
  static_assert(!is_dereferenceable<invalid>::value, "");
  static_assert(!is_dereferenceable<void() const>::value, "");
  static_assert(!is_dereferenceable<void()&>::value, "");
  static_assert(!is_dereferenceable<void() &&>::value, "");
  static_assert(!is_dereferenceable<void() volatile>::value, "");
  static_assert(!is_dereferenceable<void (invalid::*)()>::value, "");

  static_assert(!is_dereferenceable<std::unique_ptr<void>>::value, "");

  static_assert(is_dereferenceable<struct incomplete*>::value, "");
  static_assert(is_dereferenceable<char*>::value, "");
  static_assert(is_dereferenceable<std::unique_ptr<char>>::value, "");
  static_assert(is_dereferenceable<void()>::value, "");
  static_assert(is_dereferenceable<void (*)()>::value, "");
  static_assert(is_dereferenceable<void (&)()>::value, "");
}
