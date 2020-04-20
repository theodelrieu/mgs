#include <fstream>
#include <random>
#include <string>
#include <vector>

#include <catch2/catch.hpp>
#include <mgs/meta/concepts/output_range.hpp>

#include <mgs/base64.hpp>
#include <mgs/codecs/detail/read_at_most.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>

#include "codec_helpers.hpp"
#include "noop_iterator.hpp"

using namespace std::string_literals;
using namespace mgs;

extern std::vector<std::string> testFilePaths;

TEST_CASE("base64")
{
  std::vector<std::string> decoded{"abcd"s, "abcde"s, "abcdef"s};
  std::vector<std::string> encoded{"YWJjZA=="s, "YWJjZGU="s, "YWJjZGVm"s};
  auto const encoded_twice = "WVdKalpHVm0="s;

  SECTION("Common tests")
  {
    CHECK(base64::encode<std::string>("") == "");
    CHECK(base64::decode<std::string>("") == "");
    for (auto i = 0u; i < encoded.size(); ++i)
    {
      test_helpers::basic_codec_tests<base64>(decoded[i], encoded[i]);
      test_helpers::test_std_containers<base64>(decoded[i], encoded[i]);
      test_helpers::test_input_streams<base64>(decoded[i], encoded[i]);
      test_helpers::test_back_and_forth<base64>(decoded[i], encoded[i]);
    }

    test_helpers::test_encode_twice<base64>(decoded.back(), encoded_twice);
  }

  SECTION("stream")
  {
    REQUIRE(testFilePaths.size() == 2);
    std::ifstream random_data(testFilePaths[0],
                              std::ios::binary | std::ios::in);
    std::ifstream b64_random_data(testFilePaths[1],
                                  std::ios::binary | std::ios::in);

    using iterator = std::istreambuf_iterator<char>;

    auto encoder = base64::make_encoder(iterator(random_data), iterator());
    auto enc_range = codecs::make_input_range(std::move(encoder));
    test_helpers::check_equal(enc_range.begin(),
                              enc_range.end(),
                              iterator(b64_random_data),
                              iterator());

    random_data.seekg(0);
    b64_random_data.seekg(0);

    auto decoder = base64::make_decoder(iterator(b64_random_data), iterator());
    auto dec_range = codecs::make_input_range(std::move(decoder));
    test_helpers::check_equal(
        dec_range.begin(), dec_range.end(), iterator{random_data}, iterator());
  }

  SECTION("invalid input")
  {
    std::string corner_case(256, 'A');
    corner_case.back() = '=';
    corner_case.push_back('A');
    std::vector<std::string> invalid_chars{"===="s,
                                           "*AAA"s,
                                           "Y==="s,
                                           "ZA==YWJj"s,
                                           "YW=j"s,
                                           "ZA===AAA"s,
                                           "ZAW@"s,
                                           corner_case};
    std::vector<std::string> invalid_eof{"YWJ"s, "YWJjZ"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base64::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base64::decode(elem),
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
        auto enc = base64::make_encoder(decoded);

        CHECK(enc.max_remaining_size() == 20);
      }

      SECTION("Huge string")
      {
        std::string huge_str(10000, 0);
        auto enc = base64::make_encoder(huge_str);

        CHECK(enc.max_remaining_size() == 13336);
      }
    }

    SECTION("decoder")
    {
      SECTION("Small string")
      {
        auto const encoded = "WVdKalpHVT0="s;

        auto dec = base64::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 9);
        dec.read(test_helpers::noop_iterator{}, 5);
        CHECK(dec.max_remaining_size() == 3);
      }

      SECTION("Huge string")
      {
        auto const encoded =
            base64::encode<std::string>(std::string(10000, '0'));

        auto dec = base64::make_decoder(encoded);
        CHECK(dec.max_remaining_size() == 10002);

        detail::read_at_most(dec, test_helpers::noop_iterator{}, 9984);
        CHECK(dec.max_remaining_size() == 18);
      }

      SECTION("Rounded size")
      {
        auto encoded = base64::encode<std::string>(std::string(10000, 0));
        encoded.pop_back();

        auto dec = base64::make_decoder(encoded.begin(), encoded.end());
        CHECK(dec.max_remaining_size() == 9999);
      }

      SECTION("Invalid size")
      {
        std::string invalid(3, 0);
        auto dec = base64::make_decoder(invalid);
        CHECK(dec.max_remaining_size() == 0);
      }
    }
  }

  SECTION("encoded_size")
  {
    CHECK(base64::encoded_size(0) == 0);
    CHECK(base64::encoded_size(1) == 4);
    CHECK(base64::encoded_size(2) == 4);
    CHECK(base64::encoded_size(3) == 4);
    CHECK(base64::encoded_size(4) == 8);
    CHECK(base64::encoded_size(5) == 8);
    CHECK(base64::encoded_size(6) == 8);
  }

  SECTION("max_decoded_size")
  {
    CHECK(base64::max_decoded_size(0) == 0);
    CHECK(base64::max_decoded_size(4) == 3);
    CHECK(base64::max_decoded_size(8) == 6);
    CHECK(base64::max_decoded_size(32) == 24);

    CHECK(base64::max_decoded_size(1) == -1);
    CHECK(base64::max_decoded_size(2) == -1);
    CHECK(base64::max_decoded_size(3) == -1);
    CHECK(base64::max_decoded_size(5) == -1);
    CHECK(base64::max_decoded_size(6) == -1);
    CHECK(base64::max_decoded_size(7) == -1);
    CHECK(base64::max_decoded_size(33) == -1);
    CHECK(base64::max_decoded_size(31) == -1);
  }
}
