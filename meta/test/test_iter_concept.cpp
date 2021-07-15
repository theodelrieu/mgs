#include <iterator>
#include <list>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/iter_concept.hpp>

using namespace mgs::meta;

namespace
{
struct invalid_iterator
{
};

struct valid
{
  using iterator_concept = void;
};
}

// only (?) case where iter_concept does not name a type
// is when an invalid iterator_traits specialization is provided.
namespace std
{
template <>
struct iterator_traits<invalid_iterator>
{
  using value_type = void;
};
}

TEST_CASE("iter_concept")
{
  static_assert(std::is_base_of<std::random_access_iterator_tag,
                                iter_concept<char*>>::value,
                "");
  static_assert(std::is_same<iter_concept<std::list<char>::iterator>,
                             std::bidirectional_iterator_tag>::value,
                "");
  static_assert(std::is_same<iter_concept<valid>, void>::value, "");

  static_assert(!is_detected<iter_concept, invalid_iterator>::value, "");

  static_assert(
      std::is_same<iter_concept<void>, std::random_access_iterator_tag>::value,
      "");
  static_assert(
      std::is_same<iter_concept<void*>, std::random_access_iterator_tag>::value,
      "");
  // C++20 introduces contiguous iterators, use is_base_of instead of is_same
  static_assert(std::is_base_of<std::random_access_iterator_tag,
                                iter_concept<struct incomplete>>::value,
                "");
  static_assert(std::is_base_of<std::random_access_iterator_tag,
                                iter_concept<struct incomplete*>>::value,
                "");
  static_assert(
      std::is_same<iter_traits<struct incomplete>, struct incomplete>::value,
      "");
  static_assert(std::is_same<iter_traits<struct incomplete*>,
                             std::iterator_traits<struct incomplete*>>::value,
                "");
}
