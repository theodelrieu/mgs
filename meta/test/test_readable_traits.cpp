#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/iter_reference_t.hpp>
#include <mgs/meta/iter_rvalue_reference_t.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/readable_traits.hpp>

using namespace mgs::meta;

TEST_CASE("readable_traits")
{
  static_assert(!is_detected<iter_value_t, void>::value, "");
  static_assert(!is_detected<iter_value_t, struct incomplete>::value, "");
  static_assert(!is_detected<iter_value_t, void*>::value, "");

  static_assert(
      std::is_same<iter_value_t<struct incomplete*>, struct incomplete>::value,
      "");
  static_assert(std::is_same<iter_value_t<std::vector<int>>, int>::value, "");
  static_assert(std::is_same<iter_value_t<std::unique_ptr<int>>, int>::value, "");
  static_assert(std::is_same<iter_value_t<int const volatile*>, int>::value,
                "");
  static_assert(
      std::is_same<iter_value_t<std::back_insert_iterator<std::vector<int>>>,
                   void>::value,
      "");

  static_assert(!is_detected<iter_reference_t, void>::value, "");
  static_assert(!is_detected<iter_reference_t, void*>::value, "");
  static_assert(std::is_same<iter_reference_t<struct incomplete*>,
                             struct incomplete&>::value,
                "");
  static_assert(
      std::is_same<iter_reference_t<std::unique_ptr<int>>, int&>::value, "");
  static_assert(std::is_same<iter_reference_t<std::ostreambuf_iterator<char>>,
                             std::ostreambuf_iterator<char>&>::value,
                "");

  static_assert(!is_detected<iter_rvalue_reference_t, void>::value, "");
  static_assert(!is_detected<iter_rvalue_reference_t, void*>::value, "");
  static_assert(std::is_same<iter_rvalue_reference_t<struct incomplete*>,
                             struct incomplete&&>::value,
                "");

  static_assert(
      std::is_same<iter_rvalue_reference_t<char const*>, char const&&>::value,
      "");
  static_assert(
      std::is_same<iter_rvalue_reference_t<std::vector<std::string>::iterator>,
                   std::string&&>::value,
      "");
}
