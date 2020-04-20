#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/base16.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "codec_helpers.hpp"
#include "noop_iterator.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

TEST_CASE("base16")
{
  std::vector<std::string> decoded{
      "f"s, "fo"s, "foo"s, "foob"s, "fooba"s, "foobar"s};
  std::vector<std::string> encoded{
      "66"s,
      "666F"s,
      "666F6F"s,
      "666F6F62"s,
      "666F6F6261"s,
      "666F6F626172"s,
  };
  auto const encoded_lower = "666f6f626172"s;
  auto const encoded_twice = "363636463646363236313732"s;

  SECTION("Common tests")
  {
    CHECK(base16::encode<std::string>("") == "");
    CHECK(base16::decode<std::string>("") == "");
    for (auto i = 0u; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base16>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base16>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base16>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base16>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base16>(decoded.back(), encoded_twice);
    test_helpers::test_decode<base16>(encoded_lower, decoded.back());
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0],
                              std::ios::binary | std::ios::in);
    std::ifstream b16_random_data(testFilePaths[1],
                                  std::ios::binary | std::ios::in);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base16::make_encoder(iterator(random_data), iterator());
    auto range = codecs::make_input_range(std::move(encoder));
    test_helpers::check_equal(
        range.begin(), range.end(), iterator(b16_random_data), iterator());

    random_data.seekg(0);
    b16_random_data.seekg(0);

    auto decoder = base16::make_decoder(iterator(b16_random_data), iterator());
    auto range2 = codecs::make_input_range(std::move(decoder));

    test_helpers::check_equal(
        range2.begin(), range2.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"=="s, "**"s, "0G"s};
    std::vector<std::string> invalid_eof{"0F0"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base16::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base16::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
  }

  SECTION("max_remaining_size")
  {
    SECTION("encoder")
    {
      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base16::make_encoder(decoded.begin(), decoded.end());

        CHECK(enc.max_remaining_size() == 26);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base16::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_remaining_size() == 20000);
      }
    }

    SECTION("decoder")
    {
      SECTION("Small string")
      {
        auto const encoded = "666F6F62"s;

        auto dec = base16::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 4);
        dec.read(test_helpers::noop_iterator{}, 2);
        CHECK(dec.max_remaining_size() == 2);
      }

      SECTION("Huge string")
      {
        auto const encoded = base16::encode<std::string>(std::string(10000, 0));

        auto dec = base16::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 10000);
      }

      SECTION("Rounded size")
      {
        auto encoded = base16::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base16::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 9999);
      }

      SECTION("Invalid size")
      {
        std::string invalid(1, 0);

        auto dec = base16::make_decoder(invalid);
        CHECK(dec.max_remaining_size() == 0);
      }
    }
  }

  SECTION("encoded_size")
  {
    CHECK(base16::encoded_size(0) == 0);
    CHECK(base16::encoded_size(1) == 2);
    CHECK(base16::encoded_size(2) == 4);
    CHECK(base16::encoded_size(3) == 6);
    CHECK(base16::encoded_size(4) == 8);
    CHECK(base16::encoded_size(5) == 10);
    CHECK(base16::encoded_size(8) == 16);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base16::max_decoded_size(0) == 0);
    CHECK(base16::max_decoded_size(2) == 1);
    CHECK(base16::max_decoded_size(4) == 2);
    CHECK(base16::max_decoded_size(32) == 16);

    CHECK(base16::max_decoded_size(1) == -1);
    CHECK(base16::max_decoded_size(3) == -1);
    CHECK(base16::max_decoded_size(33) == -1);
  }
}
