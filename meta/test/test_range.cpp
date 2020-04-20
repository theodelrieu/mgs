#include <forward_list>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/bidirectional_range.hpp>
#include <mgs/meta/concepts/common_range.hpp>
#include <mgs/meta/concepts/forward_range.hpp>
#include <mgs/meta/concepts/input_range.hpp>
#include <mgs/meta/concepts/random_access_range.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/weakly_equality_comparable_with.hpp>

using namespace mgs::meta;

namespace
{
struct non_range
{
};

struct invalid_range
{
};

struct invalid_sentinel
{
};

char* begin(invalid_range&);
invalid_sentinel end(invalid_range&);

struct valid_common_range
{
};

char* begin(valid_common_range&);
char* end(valid_common_range&);

struct valid_input_range
{
};

std::istreambuf_iterator<char> begin(valid_input_range&);
std::istreambuf_iterator<char> end(valid_input_range&);

struct valid_output_common_range
{
};

struct valid_output_iterator
{
  using difference_type = std::ptrdiff_t;

  valid_output_iterator& operator++();
  valid_output_iterator& operator++(int);
  valid_output_iterator& operator*();

  template <typename T>
  valid_output_iterator& operator=(T&&);
};

bool operator==(valid_output_iterator, valid_output_iterator);
bool operator!=(valid_output_iterator, valid_output_iterator);

valid_output_iterator begin(valid_output_common_range&);
valid_output_iterator end(valid_output_common_range&);

char* begin(valid_common_range&);
char* end(valid_common_range&);

struct sentinel
{
};

bool operator==(char*, sentinel);
bool operator!=(char*, sentinel);
bool operator==(sentinel, char*);
bool operator!=(sentinel, char*);

struct sentinel_range
{
};

char* begin(sentinel_range&);
sentinel end(sentinel_range&);
}

TEST_CASE("range")
{
  static_assert(is_range<char[1]>::value, "");
  static_assert(is_range<char const[1]>::value, "");
  static_assert(is_range<std::string>::value, "");
  static_assert(is_range<std::vector<int>>::value, "");
  static_assert(is_range<valid_common_range>::value, "");
  static_assert(is_range<sentinel_range>::value, "");
  static_assert(is_common_range<valid_common_range>::value, "");
  static_assert(is_common_range<valid_output_common_range>::value, "");
  static_assert(is_input_range<std::string>::value, "");
  static_assert(is_input_range<valid_input_range>::value, "");
  static_assert(is_forward_range<std::string>::value, "");
  static_assert(is_bidirectional_range<std::string>::value, "");
  static_assert(is_bidirectional_range<std::list<char>>::value, "");
  static_assert(is_random_access_range<std::string>::value, "");

  static_assert(!is_forward_range<valid_input_range>::value, "");
  static_assert(!is_bidirectional_range<std::forward_list<char>>::value, "");
  static_assert(!is_random_access_range<std::list<char>>::value, "");
  static_assert(!is_input_range<valid_output_common_range>::value, "");
  static_assert(!is_common_range<sentinel_range>::value, "");
  static_assert(!is_range<char*>::value, "");
  static_assert(!is_range<non_range>::value, "");
  static_assert(!is_range<invalid_range>::value, "");

  static_assert(!is_forward_range<struct incomplete>::value, "");
  static_assert(!is_forward_range<void>::value, "");
  static_assert(!is_input_range<struct incomplete>::value, "");
  static_assert(!is_input_range<void>::value, "");
  static_assert(!is_range<struct incomplete>::value, "");
  static_assert(!is_range<void>::value, "");
}
