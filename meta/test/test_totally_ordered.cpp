#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/totally_ordered.hpp>
#include <mgs/meta/concepts/totally_ordered_with.hpp>

using namespace mgs::meta;

namespace
{
struct almost_correct
{
};

bool operator<(almost_correct, almost_correct);
bool operator>(almost_correct, almost_correct);
bool operator>=(almost_correct, almost_correct);

struct correct
{
};

bool operator==(correct, correct);
bool operator!=(correct, correct);
bool operator<(correct, correct);
bool operator>(correct, correct);
bool operator>=(correct, correct);
bool operator<=(correct, correct);
}

TEST_CASE("totally_ordered")
{
  static_assert(is_totally_ordered<int>::value, "");
  static_assert(is_totally_ordered<correct>::value, "");
  static_assert(!is_totally_ordered<almost_correct>::value, "");
  static_assert(!is_totally_ordered<void>::value, "");
}

TEST_CASE("totally_ordered_with")
{
  static_assert(is_totally_ordered_with<int, float>::value, "");
  static_assert(is_totally_ordered_with<correct, correct>::value, "");
  static_assert(
      !is_totally_ordered_with<almost_correct, almost_correct>::value,
      "");
  static_assert(!is_totally_ordered_with<void, void>::value, "");
}
