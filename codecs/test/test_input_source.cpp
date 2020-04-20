#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/meta/concepts/random_access_iterator.hpp>
#include <mgs/meta/concepts/semiregular.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct valid_input_source
{
  using element_type = char;

  valid_input_source() = default;
  valid_input_source(valid_input_source const&) = delete;
  valid_input_source(valid_input_source&&) = default;
  valid_input_source& operator=(valid_input_source const&) = delete;
  valid_input_source& operator=(valid_input_source&&) = default;

  std::pair<char*, int> read(char*, int);
};

struct non_movable_input_source
{
  using element_type = char;

  non_movable_input_source(non_movable_input_source const&) = default;
  non_movable_input_source(non_movable_input_source &&) = delete;

  std::pair<char*, int>read(char*, int);
};

struct no_element_type_input_source
{
  std::pair<char*, int> read(char*, int);
};

struct invalid_read_input_source
{
  using element_type = char;

  std::string read(char*, int);
};

struct invalid_element_type_input_source
{
  using element_type = double;

  std::pair<char*, int>  read(char*, int);
};
}

TEST_CASE("input_source")
{
  static_assert(is_input_source<valid_input_source>::value, "");

  static_assert(!is_input_source<struct incomplete>::value, "");
  static_assert(!is_input_source<void>::value, "");
  static_assert(!is_input_source<void*>::value, "");

  static_assert(!is_input_source<non_movable_input_source>::value, "");
  static_assert(!is_input_source<no_element_type_input_source>::value, "");
  static_assert(
      !is_input_source<invalid_read_input_source>::value, "");
  static_assert(!is_input_source<invalid_element_type_input_source>::value, "");
}
