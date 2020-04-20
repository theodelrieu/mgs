#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/common_reference.hpp>
#include <mgs/meta/concepts/common_reference_with.hpp>
#include <mgs/meta/detected.hpp>

using namespace mgs::meta;

// Use a part of the range-v3 test suite for this specific trait:
namespace
{
template <typename T>
struct S;
struct B
{
};

struct D : B
{
};
}

TEST_CASE("common_reference_with")
{
  static_assert(!is_detected<common_reference_t>::value, "");
  static_assert(
      !is_detected<common_reference_t, int, struct incomplete*>::value, "");

  static_assert(std::is_same<common_reference_t<int>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, int>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, int, int>, int>::value,
                "");
  static_assert(has_common_reference_with<int, int>::value, "");

  static_assert(std::is_same<common_reference_t<int const&, int volatile&>,
                             int const volatile&>::value,
                "");
  static_assert(has_common_reference_with<int const&, int volatile&>::value,
                "");

  static_assert(std::is_same<common_reference_t<B&, D&>, B&>::value, "");
  static_assert(has_common_reference_with<B&, D&>::value, "");
  static_assert(std::is_same<common_reference_t<B&, D const&>, B const&>::value,
                "");
  static_assert(has_common_reference_with<B&, D const&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&, D const&, D&>, B const&>::value, "");
  static_assert(std::is_same<common_reference_t<B const&, D&>, B const&>::value,
                "");
  static_assert(has_common_reference_with<B const&, D&>::value, "");
  static_assert(std::is_same<common_reference_t<B&, D&, B&, D&>, B&>::value,
                "");

  static_assert(std::is_same<common_reference_t<B&&, D&&>, B&&>::value, "");
  static_assert(has_common_reference_with<B&&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&&, D&&>, B const&&>::value, "");
  static_assert(has_common_reference_with<B const&&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&&, D const&&>, B const&&>::value, "");
  static_assert(has_common_reference_with<B&&, D const&&>::value, "");

  static_assert(std::is_same<common_reference_t<B&, D&&>, B const&>::value, "");
  static_assert(has_common_reference_with<B&, D&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&, D const&&>, B const&>::value, "");
  static_assert(has_common_reference_with<B&, D const&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&, D&&>, B const&>::value, "");
  static_assert(has_common_reference_with<B const&, D&&>::value, "");

  static_assert(std::is_same<common_reference_t<B&&, D&>, B const&>::value, "");
  static_assert(has_common_reference_with<B&&, D&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B&&, D const&>, B const&>::value, "");
  static_assert(has_common_reference_with<B&&, D const&>::value, "");
  static_assert(
      std::is_same<common_reference_t<B const&&, D&>, B const&>::value, "");
  static_assert(has_common_reference_with<B const&&, D&>::value, "");

  static_assert(std::is_same<common_reference_t<int, short>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int, short&>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int&, short&>, int>::value, "");
  static_assert(std::is_same<common_reference_t<int&, short>, int>::value, "");
  static_assert(has_common_reference_with<int, short>::value, "");
  static_assert(has_common_reference_with<int, short&>::value, "");
  static_assert(has_common_reference_with<int&, short&>::value, "");
  static_assert(has_common_reference_with<int&, short>::value, "");

  // tricky volatile reference case
  // common reference with both volatile are too hard on MSVC
#ifndef _MSC_VER
  static_assert(has_common_reference_with<int volatile&&, int volatile&>::value,
                "");

  static_assert(std::is_same<common_reference_t<int volatile&&, int volatile&>,
                             int>::value,
                "");
#endif
  static_assert(
      std::is_same<common_reference_t<int&&, int volatile&>, int>::value, "");
  static_assert(has_common_reference_with<int&&, int volatile&>::value, "");
  static_assert(
      std::is_same<common_reference_t<int volatile&, int&&>, int>::value, "");
  static_assert(has_common_reference_with<int volatile&, int&&>::value, "");
  static_assert(
      std::is_same<common_reference_t<int const volatile&&, int volatile&&>,
                   int const volatile&&>::value,
      "");
  static_assert(
      has_common_reference_with<int const volatile&&, int volatile&&>::value,
      "");
  static_assert(
      std::is_same<common_reference_t<int&&, int const&, int volatile&>,
                   int const volatile&>(),
      "");

  // Array types?? Yup! Except for MSVC which has issues with COND_RES
  // implementation
#ifndef _MSC_VER
  static_assert(!is_detected<common_reference_t, int, char(&)[]>::value, "");
  static_assert(!is_detected<common_reference_t, int, void const>::value, "");
  static_assert(!is_detected<common_reference_t, void, int const>::value, "");
  static_assert(
      std::is_same<common_reference_t<void const, void volatile>, void>::value,
      "");
  static_assert(std::is_same<common_reference_t<int(&)[10], int(&&)[10]>,
                             int const(&)[10]>::value,
                "");
  static_assert(has_common_reference_with<int(&)[10], int(&&)[10]>::value, "");
  static_assert(
      std::is_same<common_reference_t<int const(&)[10], int volatile(&)[10]>,
                   int const volatile(&)[10]>::value,
      "");
  static_assert(
      has_common_reference_with<int const(&)[10], int volatile(&&)[10]>::value,
      "");
  static_assert(
      std::is_same<common_reference_t<int(&)[10], int(&)[11]>, int*>::value,
      "");
  static_assert(has_common_reference_with<int(&)[10], int(&)[11]>::value, "");
#endif
}
