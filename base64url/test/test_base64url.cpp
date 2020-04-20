#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/base64url.hpp>
#include <mgs/codecs/detail/read_at_most.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "codec_helpers.hpp"
#include "noop_iterator.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

TEST_CASE("base64url")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};
  auto const encoded_twice = "WVdKalpHVm0="s;

  SECTION("Common tests")
  {
    CHECK(base64url::encode<std::string>("") == "");
    CHECK(base64url::decode<std::string>("") == "");
    for (auto i = 0u; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base64url>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base64url>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base64url>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base64url>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base64url>(decoded.back(), encoded_twice);
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0],
                              std::ios::binary | std::ios::in);
    std::ifstream b64_random_data(testFilePaths[1],
                                  std::ios::binary | std::ios::in);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base64url::make_encoder(iterator(random_data), iterator());
    auto range = codecs::make_input_range(std::move(encoder));
    test_helpers::check_equal(
        range.begin(), range.end(), iterator(b64_random_data), iterator());

    random_data.seekg(0);
    b64_random_data.seekg(0);

    auto decoder =
        base64url::make_decoder(iterator(b64_random_data), iterator());
    auto range2 = codecs::make_input_range(std::move(decoder));
    test_helpers::check_equal(
        range.begin(), range.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA+="s, "YW/j"s, "ZA===AAA"s, "ZAW@"s};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base64url::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base64url::decode(elem),
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
        auto enc = base64url::make_encoder(decoded);

        CHECK(enc.max_remaining_size() == 20);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base64url::make_encoder(huge_str);

        CHECK(enc.max_remaining_size() == 13336);
      }
    }

    SECTION("decoder")
    {
      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;

        auto dec = base64url::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 9);
        dec.read(test_helpers::noop_iterator{}, 5);
        CHECK(dec.max_remaining_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base64url::encode<std::string>(std::string(10000, 0));

        auto dec = base64url::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 10002);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        detail::read_at_most(dec, test_helpers::noop_iterator{}, 9984);
        CHECK(dec.max_remaining_size() == 18);
      }

      SECTION("Rounded size")
      {
        auto encoded = base64url::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base64url::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 9999);
      }

      SECTION("Invalid size")
      {
        std::string invalid(3, 0);
        auto dec = base64url::make_decoder(invalid);
        CHECK(dec.max_remaining_size() == 0);
      }
    }
  }

  SECTION("encoded_size")
  {
    CHECK(base64url::encoded_size(0) == 0);
    CHECK(base64url::encoded_size(1) == 4);
    CHECK(base64url::encoded_size(2) == 4);
    CHECK(base64url::encoded_size(3) == 4);
    CHECK(base64url::encoded_size(4) == 8);
    CHECK(base64url::encoded_size(5) == 8);
    CHECK(base64url::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64url::max_decoded_size(0) == 0);
    CHECK(base64url::max_decoded_size(4) == 3);
    CHECK(base64url::max_decoded_size(8) == 6);
    CHECK(base64url::max_decoded_size(32) == 24);

    CHECK(base64url::max_decoded_size(1) == -1);
    CHECK(base64url::max_decoded_size(2) == -1);
    CHECK(base64url::max_decoded_size(3) == -1);
    CHECK(base64url::max_decoded_size(5) == -1);
    CHECK(base64url::max_decoded_size(6) == -1);
    CHECK(base64url::max_decoded_size(7) == -1);
    CHECK(base64url::max_decoded_size(33) == -1);
    CHECK(base64url::max_decoded_size(31) == -1);
  }
}

TEST_CASE("base64url_nopad", "[base64url]")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded_padded{
      "YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};
  std::vector<std::string> encoded_unpadded{"YWJjZA"s, "YWJjZGU"s, "YWJjZGVm"s};
  auto const encoded_twice = "WVdKalpHVm0"s;

  SECTION("Common tests")
  {
    for (auto i = 0u; i < encoded_unpadded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base64url_nopad>(decoded[i],
                                                       encoded_unpadded[i]);
      test_helpers::test_std_containers<base64url_nopad>(decoded[i],
                                                         encoded_unpadded[i]);
      test_helpers::test_input_streams<base64url_nopad>(decoded[i],
                                                        encoded_unpadded[i]);
      test_helpers::test_back_and_forth<base64url_nopad>(decoded[i],
                                                         encoded_unpadded[i]);
    }

    test_helpers::test_encode_twice<base64url_nopad>(decoded.back(),
                                                     encoded_twice);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{
        "===="s, "*AAA"s, "Y==="s, "ZA==YWJj"s, "YW=j"s, "ZA===AAA"s, "ZAW@"s};

    std::vector<std::string> invalid_eof{"Y"s, "YWJjZ"s};
    std::vector<std::string> invalid_nopad_chars{"ZAAAA="s, "ZAW@=="s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base64url_nopad::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base64url_nopad::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
    for (auto const& elem : invalid_nopad_chars)
    {
      CHECK_THROWS_AS(base64url_nopad::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }
  }

  SECTION("max_remaining_size")
  {
    SECTION("encoder")
    {
      SECTION("Small string")
      {
        auto const decoded = "abcdefghijklm"s;
        auto enc = base64url_nopad::make_encoder(decoded);

        CHECK(enc.max_remaining_size() == 18);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base64url_nopad::make_encoder(huge_str);

        CHECK(enc.max_remaining_size() == 13334);
      }
    }

    SECTION("decoder")
    {
      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;

        auto dec = base64url_nopad::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 9);
        dec.read(test_helpers::noop_iterator{}, 5);
        CHECK(dec.max_remaining_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base64url_nopad::encode<std::string>(std::string(10000, 0));

        auto dec = base64url_nopad::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 10000);

        // trigger last decode operation, padding is removed, exact size is
        // returned
        detail::read_at_most(dec, test_helpers::noop_iterator{}, 9984);
        CHECK(dec.max_remaining_size() == 16);
      }

      SECTION("Invalid size")
      {
        std::string invalid(1, 0);
        auto dec = base64url_nopad::make_decoder(invalid);
        CHECK(dec.max_remaining_size() == 0);
      }
    }
  }

  SECTION("encoded_size")
  {
    CHECK(base64url_nopad::encoded_size(0) == 0);
    CHECK(base64url_nopad::encoded_size(1) == 2);
    CHECK(base64url_nopad::encoded_size(2) == 3);
    CHECK(base64url_nopad::encoded_size(3) == 4);
    CHECK(base64url_nopad::encoded_size(4) == 6);
    CHECK(base64url_nopad::encoded_size(5) == 7);
    CHECK(base64url_nopad::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64url_nopad::max_decoded_size(0) == 0);
    CHECK(base64url_nopad::max_decoded_size(2) == 1);
    CHECK(base64url_nopad::max_decoded_size(3) == 2);
    CHECK(base64url_nopad::max_decoded_size(4) == 3);
    CHECK(base64url_nopad::max_decoded_size(6) == 4);
    CHECK(base64url_nopad::max_decoded_size(7) == 5);
    CHECK(base64url_nopad::max_decoded_size(8) == 6);
    CHECK(base64url_nopad::max_decoded_size(22) == 16);
    CHECK(base64url_nopad::max_decoded_size(24) == 18);

    CHECK(base64url_nopad::max_decoded_size(1) == -1);
    CHECK(base64url_nopad::max_decoded_size(5) == -1);
    CHECK(base64url_nopad::max_decoded_size(21) == -1);
  }
}
