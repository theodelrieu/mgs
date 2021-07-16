#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/equality_comparable.hpp>
#include <mgs/meta/concepts/regular.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/swappable.hpp>

using namespace mgs::meta;

namespace
{
struct almost_semiregular
{
  almost_semiregular() = delete;
};

bool operator==(almost_semiregular const&, almost_semiregular const&);
bool operator!=(almost_semiregular const&, almost_semiregular const&);

struct valid_semiregular
{
};

struct almost_regular
{
};

bool operator==(almost_regular const&, almost_regular const&);

// Have to explicitly delete it since C++20 introduces default comparisons
bool operator!=(almost_regular const&, almost_regular const&) = delete;

struct valid_regular
{
};

bool operator==(valid_regular const&, valid_regular const&);
bool operator!=(valid_regular const&, valid_regular const&);
}

TEST_CASE("regular")
{
  static_assert(is_regular<int>::value, "");
  static_assert(!is_regular<valid_semiregular>::value, "");
  static_assert(is_regular<valid_regular>::value, "");
  static_assert(!is_regular<almost_regular>::value, "");

  static_assert(!is_regular<void>::value, "");
  static_assert(!is_regular<struct incomplete>::value, "");
}
