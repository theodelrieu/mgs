#include <catch2/catch.hpp>

#include <algorithm>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <utility>

#include <mgs/codecs/basic_codec.hpp>
#include <mgs/codecs/concepts/codec.hpp>
#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "codec_helpers.hpp"

using namespace mgs;
using namespace mgs::codecs;
using namespace std::string_literals;

namespace
{
struct invalid_type
{
};

struct valid_type
{
  std::vector<std::uint8_t> vec;

  auto begin()
  {
    return vec.begin();
  }

  auto end()
  {
    return vec.end();
  }

  auto begin() const
  {
    return vec.begin();
  }

  auto end() const
  {
    return vec.end();
  }
};

struct valid_codec_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename IS>
  // only works thanks to iterator_sentinel_source being copyable
  static auto make_encoder(IS is)
  {
    return std::move(is);
  }

  template <typename IS>
  static auto make_decoder(IS is)
  {
    return std::move(is);
  }
};

using valid_codec = codecs::basic_codec<valid_codec_traits>;
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid_type>
{
  template <typename IS>
  static valid_type create(IS is)
  {
    return {{is.begin(), is.end()}};
  }
};

template <typename T>
struct output_traits<std::vector<T>>
{
  template <typename IS>
  static std::vector<T> create(IS is)
  {
    return {is.begin(), is.end()};
  }
};
}
}

TEST_CASE("codecs")
{
  SECTION("codec output")
  {
    std::array<char, 4> const input{'t', 'e', 's', 't'};

    using Encoder = decltype(valid_codec::make_encoder(
        std::declval<iterator_sentinel_source<decltype(input.begin())>&>()));

    static_assert(!is_codec_output<invalid_type, Encoder>::value, "");
    static_assert(is_codec_output<valid_type, Encoder>::value, "");

    SECTION("C arrays")
    {
      unsigned char tab[10] = {};
      char tab2[10] = {};
      char tab3[10] = "abcdefghi";

      valid_codec::encode(""s);
      SECTION("encode")
      {
        auto const v = valid_codec::encode(tab);
        auto const v2 = valid_codec::encode(tab2);
        auto const v3 = valid_codec::encode(tab3);
        auto const v5 = valid_codec::encode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v5.size() == 9);
      }

      SECTION("decode")
      {
        auto const v = valid_codec::decode(tab);
        auto const v2 = valid_codec::decode(tab2);
        auto const v3 = valid_codec::decode(tab3);
        auto const v5 = valid_codec::decode("abcdefghi");

        CHECK(v.size() == 10);
        CHECK(v2.size() == 0);
        CHECK(v3.size() == 9);
        CHECK(v5.size() == 9);
      }
    }

    SECTION("User-defined types")
    {
      test_helpers::basic_codec_tests<valid_codec, valid_type>(input, input);
    }

    SECTION("Common tests")
    {
      test_helpers::basic_codec_tests<valid_codec>(input, input);

      test_helpers::test_std_containers<valid_codec>(input, input);
      test_helpers::test_input_streams<valid_codec>(input, input);
      test_helpers::test_back_and_forth<valid_codec>(input, input);
      test_helpers::test_encode_twice<valid_codec>(input, input);
    }

    SECTION("Array conversion")
    {
      CHECK_THROWS_AS((valid_codec::encode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::encode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::decode<std::array<char, 3>>(input)),
                      mgs::exceptions::unexpected_eof_error);
      CHECK_THROWS_AS((valid_codec::decode<std::array<char, 5>>(input)),
                      mgs::exceptions::unexpected_eof_error);
    }
  }
}
