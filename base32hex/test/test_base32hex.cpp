#include <fstream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/base32hex.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "codec_helpers.hpp"
#include "noop_iterator.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

TEST_CASE("base32hex")
{
  std::vector<std::string> decoded{"a"s, "ab"s, "abc"s, "abcd"s, "abcde"s};
  std::vector<std::string> encoded{
      "C4======"s, "C5H0===="s, "C5H66==="s, "C5H66P0="s, "C5H66P35"s};
  auto const encoded_twice = "8CQKGDHMA0PJA==="s;

  SECTION("Common tests")
  {
    CHECK(base32hex::encode<std::string>("") == "");
    CHECK(base32hex::decode<std::string>("") == "");
    for (auto i = 0u; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base32hex>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base32hex>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base32hex>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base32hex>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base32hex>(decoded.back(), encoded_twice);
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0],
                              std::ios::binary | std::ios::in);
    std::ifstream b32hex_random_data(testFilePaths[1],
                                     std::ios::binary | std::ios::in);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base32hex::make_encoder(iterator(random_data), iterator());
    auto range = codecs::make_input_range(std::move(encoder));
    test_helpers::check_equal(
        range.begin(), range.end(), iterator(b32hex_random_data), iterator());

    random_data.seekg(0);
    b32hex_random_data.seekg(0);

    auto decoder =
        base32hex::make_decoder(iterator(b32hex_random_data), iterator());
    auto range2 = codecs::make_input_range(std::move(decoder));
    test_helpers::check_equal(
        range2.begin(), range2.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"========"s,
                                           "**+====/"s,
                                           "V======="s,
                                           "ABC====="s,
                                           "ABCDEF=="s,
                                           "ABCDE@=="s};
    std::vector<std::string> invalid_eof{"ABC"s, "ABDHCAV"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base32hex::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base32hex::decode(elem),
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
        auto enc = base32hex::make_encoder(decoded);

        CHECK(enc.max_remaining_size() == 24);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base32hex::make_encoder(huge_str.begin(), huge_str.end());

        CHECK(enc.max_remaining_size() == 16000);
      }

      SECTION("Input source's max_remaining_size returning a rounded value")
      {
        auto dec = base32hex::make_decoder("round to 8");
        auto enc = base32hex::make_encoder(std::move(dec));
        CHECK(enc.max_remaining_size() == 8);
      }
    }

    SECTION("decoder")
    {
      SECTION("Small string")
      {
        auto const encoded = "C5H66P0="s;

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 5);
        dec.read(test_helpers::noop_iterator{}, 2);
        CHECK(dec.max_remaining_size() == 2);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base32hex::encode<std::string>(std::string(10000, 0));

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 10000);
      }

      SECTION("Rounded size")
      {
        auto encoded = base32hex::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base32hex::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 9995);
      }

      SECTION("Invalid size")
      {
        std::string invalid(4, 0);

        auto dec = base32hex::make_decoder(invalid);
        CHECK(dec.max_remaining_size() == 0);
      }

      SECTION("Input source's max_remaining_size returning a rounded value")
      {
        auto dec = base32hex::make_decoder("it will round to 16");
        auto dec2 = base32hex::make_decoder(std::move(dec));
        CHECK(dec2.max_remaining_size() == 5);
      }
    }
  }

  SECTION("encoded_size")
  {
    CHECK(base32hex::encoded_size(0) == 0);
    CHECK(base32hex::encoded_size(1) == 8);
    CHECK(base32hex::encoded_size(2) == 8);
    CHECK(base32hex::encoded_size(3) == 8);
    CHECK(base32hex::encoded_size(4) == 8);
    CHECK(base32hex::encoded_size(5) == 8);
    CHECK(base32hex::encoded_size(6) == 16);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base32hex::max_decoded_size(0) == 0);
    CHECK(base32hex::max_decoded_size(8) == 5);
    CHECK(base32hex::max_decoded_size(32) == 20);

    CHECK(base32hex::max_decoded_size(1) == -1);
    CHECK(base32hex::max_decoded_size(2) == -1);
    CHECK(base32hex::max_decoded_size(3) == -1);
    CHECK(base32hex::max_decoded_size(4) == -1);
    CHECK(base32hex::max_decoded_size(5) == -1);
    CHECK(base32hex::max_decoded_size(6) == -1);
    CHECK(base32hex::max_decoded_size(7) == -1);
    CHECK(base32hex::max_decoded_size(33) == -1);
    CHECK(base32hex::max_decoded_size(31) == -1);
  }
}
