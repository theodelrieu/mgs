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
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>

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

class string_wrapper : std::string
{
public:
  using base_t = std::string;

  string_wrapper() = default;

  template <typename I, typename S>
  string_wrapper(meta::input_iterator<I> begin, meta::sentinel_for<S, I> end)
    : base_t(begin, end)
  {
  }

  string_wrapper(base_t::value_type const* s, std::size_t size)
    : base_t(s, size)
  {
  }

  explicit string_wrapper(base_t const& s) : base_t(s)
  {
  }

  explicit string_wrapper(base_t&& s) : base_t(std::move(s))
  {
  }

  base_t const& string() const noexcept
  {
    return *this;
  }

  void swap(string_wrapper& other) noexcept(
      noexcept(std::declval<base_t&>().swap(other)))
  {
    this->base_t::swap(other);
  }

  // std::string interface

  // Member types
  using base_t::allocator_type;
  using base_t::const_iterator;
  using base_t::const_pointer;
  using base_t::const_reference;
  using base_t::const_reverse_iterator;
  using base_t::difference_type;
  using base_t::iterator;
  using base_t::pointer;
  using base_t::reference;
  using base_t::reverse_iterator;
  using base_t::size_type;
  using base_t::traits_type;
  using base_t::value_type;

  // Element access
  using base_t::at;
  using base_t::operator[];
  using base_t::back;
  using base_t::c_str;
  using base_t::data;
  using base_t::front;

  // Iterators
  using base_t::begin;
  using base_t::cbegin;
  using base_t::crbegin;
  using base_t::rbegin;

  using base_t::cend;
  using base_t::crend;
  using base_t::end;
  using base_t::rend;

  // Capacity
  using base_t::capacity;
  using base_t::empty;
  using base_t::length;
  using base_t::max_size;
  using base_t::reserve;
  using base_t::shrink_to_fit;
  using base_t::size;

  // Operations
  using base_t::append;
  using base_t::clear;
  using base_t::erase;
  using base_t::insert;
  using base_t::pop_back;
  using base_t::push_back;
  using base_t::operator+=;
  using base_t::compare;
  using base_t::copy;
  using base_t::replace;
  using base_t::resize;
  using base_t::substr;

  // Search
  using base_t::find;
  using base_t::find_first_not_of;
  using base_t::find_first_of;
  using base_t::find_last_not_of;
  using base_t::find_last_of;
  using base_t::rfind;
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
      test_helpers::basic_codec_tests<valid_codec, string_wrapper>(input,
                                                                   input);
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
