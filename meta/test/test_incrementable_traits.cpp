#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/config.hpp>
#include <mgs/meta/incrementable_traits.hpp>
#include <mgs/meta/iter_difference_t.hpp>

using namespace mgs::meta;

namespace
{
struct valid
{
};

unsigned int operator-(valid, valid);

struct both
{
  using difference_type = char;
};

unsigned int operator-(both, both);
}

TEST_CASE("incrementable_traits")
{
  static_assert(!is_detected<iter_difference_t, void>::value, "");
  static_assert(!is_detected<iter_difference_t, struct incomplete>::value, "");
  static_assert(!is_detected<iter_difference_t, void*>::value, "");

  static_assert(std::is_same<iter_difference_t<struct incomplete*>,
                             std::ptrdiff_t>::value,
                "");
  static_assert(
      std::is_same<iter_difference_t<std::vector<int>::iterator>,
                   std::iterator_traits<
                       std::vector<int>::iterator>::difference_type>::value,
      "");
  static_assert(std::is_same<iter_difference_t<int*>, std::ptrdiff_t>::value,
                "");
  static_assert(
      std::is_same<iter_difference_t<int const*>, std::ptrdiff_t>::value, "");
  static_assert(
      std::is_same<iter_difference_t<int const* const>, std::ptrdiff_t>::value,
      "");
  static_assert(std::is_same<iter_difference_t<valid>, int>::value, "");
  static_assert(std::is_same<iter_difference_t<both>, char>::value, "");
  // in C++20, difference_type should be std::ptrdiff_t, but it depends on
  // compiler support, so skip this test for now
#ifndef MGS_HAS_CPP20
  static_assert(
      std::is_same<
          iter_difference_t<std::back_insert_iterator<std::vector<int>>>,
          void>::value,
      "");
#endif
}
