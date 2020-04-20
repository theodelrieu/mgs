#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <list>
#include <sstream>
#include <type_traits>
#include <vector>

#include <mgs/codecs/basic_input_range.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>

#include <catch2/catch.hpp>

namespace test_helpers
{
template <class InputIt1,
          class S1,
          class InputIt2,
          class S2,
          class BinaryPredicate>
bool equal(
    InputIt1 first1, S1 last1, InputIt2 first2, S2 last2, BinaryPredicate p)
{
  for (; first1 != last1 && first2 != last2; ++first1, ++first2)
  {
    if (!p(*first1, *first2))
    {
      return false;
    }
  }
  return true;
}

template <typename I1, typename S1, typename I2, typename S2>
void check_equal(I1 i1, S1 s1, I2 i2, S2 s2)
{
  CHECK(test_helpers::equal(i1, s1, i2, s2, [](auto l, auto r) {
    return static_cast<unsigned char>(l) == static_cast<unsigned char>(r);
  }));
}

template <typename Range1, typename Range2>
void check_equal(Range1&& lhs, Range2&& rhs)
{
  using std::begin;
  using std::end;

  check_equal(begin(lhs), end(lhs), begin(rhs), end(rhs));
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_encoded_output,
          typename Range1,
          typename Range2>
void test_encode(Range1&& it, Range2&& expected)
{
  auto const encoded = Codec::template encode<CodecOutput>(it);
  check_equal(encoded, expected);
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_encoded_output,
          typename I,
          typename S,
          typename Range>
void test_encode(I i, S s, Range&& expected)
{
  auto const encoded = Codec::template encode<CodecOutput>(i, s);
  check_equal(encoded, expected);
}

template <typename Codec, typename I, typename S, typename Range>
void test_make_encoder(I i, S s, Range&& expected)
{
  auto is = mgs::codecs::make_iterator_sentinel_source(i, s);
  auto encoder = Codec::traits::make_encoder(is);
  check_equal(mgs::codecs::make_input_range(std::move(encoder)), expected);
}

template <typename Codec, typename I1, typename S1, typename I2, typename S2>
void test_make_encoder(I1 i1, S1 s1, I2 i2, S2 s2)
{
  auto is = mgs::codecs::make_iterator_sentinel_source(i1, s1);
  auto encoder = Codec::traits::make_encoder(is);
  auto range = mgs::codecs::make_input_range(std::move(encoder));
  CHECK(std::equal(range.begin(), range.end(), i2, s2));
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_decoded_output,
          typename Range1,
          typename Range2>
void test_decode(Range1&& it, Range2&& expected)
{
  auto const decoded = Codec::template decode<CodecOutput>(it);
  check_equal(decoded, expected);
}

template <typename Codec,
          typename CodecOutput = typename Codec::default_decoded_output,
          typename I,
          typename S,
          typename Range>
void test_decode(I i, S s, Range&& expected)
{
  auto const decoded = Codec::template decode<CodecOutput>(i, s);
  check_equal(decoded, expected);
}

template <typename Codec, typename I1, typename S1, typename I2, typename S2>
void test_make_decoder(I1 i1, S1 s1, I2 i2, S2 s2)
{
  auto is = mgs::codecs::make_iterator_sentinel_source(i1, s1);
  auto decoder = Codec::traits::make_decoder(is);
  auto range = mgs::codecs::make_input_range(std::move(decoder));

  CHECK(std::equal(range.begin(), range.end(), i2, s2));
}

template <typename Codec, typename Range1, typename Range2>
void test_back_and_forth(Range1&& decoded_input, Range2&& encoded_input)
{
  auto encoded = Codec::encode(decoded_input);
  check_equal(encoded, encoded_input);
  auto decoded = Codec::decode(encoded);
  check_equal(decoded, decoded_input);
  encoded = Codec::encode(decoded_input);
  check_equal(encoded, encoded_input);
}

template <typename Codec, typename Range1, typename Range2>
void test_encode_twice(Range1&& decoded_input, Range2&& encoded_input)
{
  auto is1 = mgs::codecs::make_iterator_sentinel_source(decoded_input);
  auto encoder = Codec::traits::make_encoder(is1);
  auto const encoded_twice = Codec::encode(std::move(encoder));
  check_equal(encoded_twice, encoded_input);
  auto is2 = mgs::codecs::make_iterator_sentinel_source(encoded_twice);
  auto decoder = Codec::traits::make_decoder(is2);
  auto const decoded = Codec::decode(std::move(decoder));
  check_equal(decoded, decoded_input);
}

template <typename Codec, typename I, typename S, typename Range>
void test_make_decoder(I i, S s, Range&& expected)
{
  auto is = mgs::codecs::make_iterator_sentinel_source(i, s);
  auto decoder = Codec::traits::make_decoder(is);
  check_equal(mgs::codecs::make_input_range(std::move(decoder)), expected);
}

template <typename Codec,
          typename EncodedOutput = typename Codec::default_encoded_output,
          typename DecodedOutput = EncodedOutput,
          typename Range1,
          typename Range2>
void basic_codec_tests(Range1&& decoded_input, Range2&& encoded_input)
{
  using std::begin;
  using std::end;

  test_encode<Codec, EncodedOutput>(decoded_input, encoded_input);
  test_make_encoder<Codec>(
      begin(decoded_input), end(decoded_input), encoded_input);

  test_decode<Codec, DecodedOutput>(encoded_input, decoded_input);
  test_make_decoder<Codec>(
      begin(encoded_input), end(encoded_input), decoded_input);
}

namespace detail
{
template <typename Codec, typename Container, typename Range1, typename Range2>
void test_std_container_input(Range1&& decoded_input, Range2&& encoded_input)
{
  using std::begin;
  using std::end;

  Container decoded(begin(decoded_input), end(decoded_input));
  Container encoded(begin(encoded_input), end(encoded_input));

  basic_codec_tests<Codec>(decoded, encoded);
}

struct stream_sentinel
{
};

bool operator==(std::istreambuf_iterator<char> lhs, stream_sentinel)
{
  return lhs.equal({});
}

bool operator==(stream_sentinel, std::istreambuf_iterator<char> rhs)
{
  return rhs.equal({});
}

bool operator!=(std::istreambuf_iterator<char> lhs, stream_sentinel s)
{
  return !(lhs == s);
}

bool operator!=(stream_sentinel s, std::istreambuf_iterator<char> rhs)
{
  return !(s == rhs);
}
}

template <typename Codec, typename Range1, typename Range2>
void test_input_streams(Range1&& decoded_input, Range2&& encoded_input)
{
  std::string decoded_str(decoded_input.begin(), decoded_input.end());
  std::string encoded_str(encoded_input.begin(), encoded_input.end());

  std::stringstream decoded_ss(decoded_str,
                               std::ios::in | std::ios::out | std::ios::binary);
  std::stringstream encoded_ss(encoded_str,
                               std::ios::in | std::ios::out | std::ios::binary);

  using iterator = std::istreambuf_iterator<char>;

  detail::stream_sentinel sentinel;

  test_encode<Codec>(iterator(decoded_ss), sentinel, encoded_input);

  decoded_ss.seekg(0);

  test_make_encoder<Codec>(iterator(decoded_ss), sentinel, encoded_input);

  test_decode<Codec>(iterator(encoded_ss), sentinel, decoded_input);

  encoded_ss.seekg(0);

  test_make_decoder<Codec>(iterator(encoded_ss), sentinel, decoded_input);
}

template <typename Codec, typename Range1, typename Range2>
void test_std_container_outputs(Range1&& decoded_input, Range2&& encoded_input)
{
  basic_codec_tests<Codec, std::string>(decoded_input, encoded_input);

  basic_codec_tests<Codec, std::vector<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::vector<unsigned char>>(decoded_input,
                                                       encoded_input);
  basic_codec_tests<Codec, std::vector<std::int8_t>>(decoded_input,
                                                     encoded_input);
  basic_codec_tests<Codec, std::vector<std::uint8_t>>(decoded_input,
                                                      encoded_input);

  basic_codec_tests<Codec, std::deque<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::deque<unsigned char>>(decoded_input,
                                                      encoded_input);
  basic_codec_tests<Codec, std::deque<std::int8_t>>(decoded_input,
                                                    encoded_input);
  basic_codec_tests<Codec, std::deque<std::uint8_t>>(decoded_input,
                                                     encoded_input);

  basic_codec_tests<Codec, std::list<char>>(decoded_input, encoded_input);
  basic_codec_tests<Codec, std::list<unsigned char>>(decoded_input,
                                                     encoded_input);
  basic_codec_tests<Codec, std::list<std::int8_t>>(decoded_input,
                                                   encoded_input);
  basic_codec_tests<Codec, std::list<std::uint8_t>>(decoded_input,
                                                    encoded_input);
}

template <typename Codec, typename Range1, typename Range2>
void test_std_container_inputs(Range1&& decoded_input, Range2&& encoded_input)
{
  detail::test_std_container_input<Codec, std::string>(decoded_input,
                                                       encoded_input);

  detail::test_std_container_input<Codec, std::vector<char>>(decoded_input,
                                                             encoded_input);
  detail::test_std_container_input<Codec, std::vector<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::vector<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::vector<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::list<char>>(decoded_input,
                                                           encoded_input);
  detail::test_std_container_input<Codec, std::list<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::list<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::list<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::forward_list<char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::forward_list<std::uint8_t>>(
      decoded_input, encoded_input);

  detail::test_std_container_input<Codec, std::deque<char>>(decoded_input,
                                                            encoded_input);
  detail::test_std_container_input<Codec, std::deque<unsigned char>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::deque<std::int8_t>>(
      decoded_input, encoded_input);
  detail::test_std_container_input<Codec, std::deque<std::uint8_t>>(
      decoded_input, encoded_input);
}

template <typename Codec, typename Range1, typename Range2>
void test_std_containers(Range1&& decoded_input, Range2&& encoded_input)
{
  test_std_container_outputs<Codec>(decoded_input, encoded_input);
  test_std_container_inputs<Codec>(decoded_input, encoded_input);
}
}
