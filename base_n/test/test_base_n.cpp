#include <algorithm>
#include <chrono>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/base_n/basic_codec.hpp>
#include <mgs/base_n/decode_algorithm.hpp>
#include <mgs/base_n/encode_algorithm.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/derived_from.hpp>

#include "codec_helpers.hpp"

using namespace std::string_literals;
using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct base2_encoding_traits
{
  using alphabet_t = char const[2];

  static constexpr alphabet_t alphabet = {'0', '1'};

  static constexpr auto const padding_policy = base_n::padding_policy::none;

  static int index_of(char c)
  {
    auto const begin = std::begin(alphabet);
    auto const end = std::end(alphabet);
    auto it = std::find(begin, end, c);
    if (it == end)
      return -1;
    return std::distance(begin, it);
  }
};

constexpr base2_encoding_traits::alphabet_t base2_encoding_traits::alphabet;

using base2 = base_n::basic_codec<base2_encoding_traits>;
}

TEST_CASE("base2")
{
  SECTION("Common tests")
  {
    auto const decoded = "abcdef"s;
    auto const encoded = "011000010110001001100011011001000110010101100110"s;
    test_helpers::basic_codec_tests<base2>(decoded, encoded);

    test_helpers::test_std_containers<base2>(decoded, encoded);
    test_helpers::test_input_streams<base2>(decoded, encoded);
    test_helpers::test_back_and_forth<base2>(decoded, encoded);
  }

  SECTION("invalid input")
  {
    std::vector<std::string> invalid_chars{"***====="s, "23423423"s};
    std::vector<std::string> invalid_eof{"1010101"s, "101010101"s};

    for (auto const& elem : invalid_chars)
    {
      CHECK_THROWS_AS(base2::decode(elem),
                      mgs::exceptions::invalid_input_error);
    }

    for (auto const& elem : invalid_eof)
    {
      CHECK_THROWS_AS(base2::decode(elem),
                      mgs::exceptions::unexpected_eof_error);
    }
  }

  SECTION("basic_codec")
  {
    SECTION("encoded_size")
    {
      CHECK(base2::encoded_size(0) == 0);
      CHECK(base2::encoded_size(1) == 8);
      CHECK(base2::encoded_size(2) == 16);
      CHECK(base2::encoded_size(8) == 64);
    }

    SECTION("max_decoded_size")
    {
      CHECK(base2::max_decoded_size(0) == 0);
      CHECK(base2::max_decoded_size(8) == 1);
      CHECK(base2::max_decoded_size(64) == 8);

      CHECK(base2::max_decoded_size(1) == -1);
      CHECK(base2::max_decoded_size(2) == -1);
      CHECK(base2::max_decoded_size(3) == -1);
      CHECK(base2::max_decoded_size(4) == -1);
      CHECK(base2::max_decoded_size(5) == -1);
      CHECK(base2::max_decoded_size(6) == -1);
      CHECK(base2::max_decoded_size(7) == -1);
    }

    SECTION("read")
    {
      auto const str = "abcd"s;
      auto const encoded_str = "01100001011000100110001101100100"s;
      auto enc = base2::make_encoder(str);

      std::string encoded;
      auto res = enc.read(std::back_inserter(encoded), 2);
      CHECK(res.second == 2);
      res = enc.read(std::back_inserter(encoded), 29);
      CHECK(res.second == 29);
      res = enc.read(std::back_inserter(encoded), 10);
      CHECK(res.second == 1);
      CHECK(encoded == encoded_str);
    }
  }
}
