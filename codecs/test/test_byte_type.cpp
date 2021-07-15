#include <catch2/catch.hpp>

#include <cstddef>
#include <cstdint>

#include <mgs/codecs/concepts/byte_type.hpp>
#include <mgs/config.hpp>

using namespace mgs::codecs;

TEST_CASE("byte_type")
{
  static_assert(!is_byte_type<int>::value, "");
  static_assert(!is_byte_type<void>::value, "");
  static_assert(!is_byte_type<struct incomplete>::value, "");

  static_assert(is_byte_type<signed char>::value, "");
  static_assert(is_byte_type<char>::value, "");
  static_assert(is_byte_type<unsigned char>::value, "");
  static_assert(is_byte_type<unsigned char>::value, "");
  static_assert(is_byte_type<std::uint8_t>::value, "");
  static_assert(is_byte_type<std::int8_t>::value, "");

#ifdef MGS_HAS_CPP17
  static_assert(is_byte_type<std::byte>::value, "");
#endif
}
