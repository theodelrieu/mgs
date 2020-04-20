#include <iterator>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/detail/iterator_traits.hpp>
#include <mgs/meta/iterator_traits.hpp>

using namespace mgs::meta;
using namespace mgs::detail;

namespace
{
struct invalid_iterator
{
};

struct valid_iterator
{
};
}

namespace std
{
template <>
struct iterator_traits<invalid_iterator>
{
};

template <>
struct iterator_traits<valid_iterator>
{
  using value_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
};
}

TEST_CASE("iterator_traits")
{
  static_assert(is_iterator_traits<std::iterator_traits<char*>>::value, "");
  static_assert(is_iterator_traits<std::iterator_traits<valid_iterator>>::value,
                "");
  static_assert(
      is_iterator_traits<std::iterator_traits<struct incomplete*>>::value, "");

  static_assert(is_iterator_traits<mgs::meta::iterator_traits<char*>>::value,
                "");
  static_assert(
      is_iterator_traits<mgs::meta::iterator_traits<valid_iterator>>::value,
      "");
  static_assert(
      is_iterator_traits<mgs::meta::iterator_traits<struct incomplete*>>::value,
      "");

  static_assert(
      !is_iterator_traits<std::iterator_traits<invalid_iterator>>::value, "");
  static_assert(!is_iterator_traits<int>::value, "");
  static_assert(!is_iterator_traits<void>::value, "");
  static_assert(!is_iterator_traits<std::iterator_traits<void*>>::value, "");
  static_assert(!is_iterator_traits<std::iterator_traits<void*>>::value, "");
  static_assert(!is_iterator_traits<std::iterator_traits<void>>::value, "");
  static_assert(
      !is_iterator_traits<std::iterator_traits<struct incomplete>>::value, "");
  static_assert(!is_iterator_traits<struct incomplete>::value, "");
}
