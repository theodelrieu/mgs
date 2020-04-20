#pragma once

#include <cstdlib>
#include <string>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/exceptions/invalid_input_error.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename ForwardIterator>
[[noreturn]] void throw_invalid_input(ForwardIterator invalid_it) {
  using namespace std::string_literals;
  throw exceptions::invalid_input_error{
      "invalid encoded character: '"s + static_cast<char>(*invalid_it) + "' (" +
      std::to_string(static_cast<int>(*invalid_it)) + ")"};
}

template <typename EncodingTraits, typename Input>
auto find_invalid_byte(Input const& input)
{
  // Input could be a detail::span, which can return a sized_sentinel_for
  // STL algorithms do not work with sentinels (yet).
  auto current = input.begin();
  auto const end = input.end();

  while (current != end)
  {
    if (EncodingTraits::index_of(static_cast<char>(*current)) == -1)
      break;
    ++current;
  }
  return current;
}

template <typename EncodingTraits,
          typename ForwardIterator,
          typename SizedSentinel>
void ensures_only_padding_characters(ForwardIterator invalid_byte_it,
                                     SizedSentinel const input_end)
{
  while (invalid_byte_it != input_end)
  {
    if (*invalid_byte_it != EncodingTraits::padding_character)
      throw_invalid_input(invalid_byte_it);
    ++invalid_byte_it;
  }
}

template <typename BitshiftTraits>
constexpr bool invalid_padding_character_pos(mgs::ssize_t nb_non_padded_bytes)
{
  auto const rem = (nb_non_padded_bytes * BitshiftTraits::nb_index_bits) % 8;
  return rem >= BitshiftTraits::nb_index_bits;
}

template <typename EncodingTraits, typename Input, typename ForwardIterator>
void sanitize_invalid_input(Input const& input, ForwardIterator invalid_byte_it)
{
  using BitshiftTraits = bitshift_traits<EncodingTraits>;

  auto const nb_non_padded_bytes = invalid_byte_it - input.begin();
  auto const nb_expected_padding_bytes =
      BitshiftTraits::nb_encoded_bytes -
      (nb_non_padded_bytes % BitshiftTraits::nb_encoded_bytes);

  if (nb_expected_padding_bytes > BitshiftTraits::nb_max_padding_bytes)
    throw_invalid_input(invalid_byte_it);
  if (invalid_padding_character_pos<BitshiftTraits>(nb_non_padded_bytes))
    throw_invalid_input(invalid_byte_it);
  if (input.end() - invalid_byte_it > nb_expected_padding_bytes)
    throw_invalid_input(invalid_byte_it);
  ensures_only_padding_characters<EncodingTraits>(invalid_byte_it, input.end());
}

template <typename EncodingTraits,
          base_n::padding_policy = EncodingTraits::padding_policy>
class input_sanitizer;

template <typename EncodingTraits>
class input_sanitizer<EncodingTraits, base_n::padding_policy::none>
{
public:
  template <typename Input>
  static mgs::ssize_t sanitize(Input const& input)
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const invalid_byte_it = find_invalid_byte<EncodingTraits>(input);
    if (invalid_byte_it != input.end())
      throw_invalid_input(invalid_byte_it);
    else if (invalid_padding_character_pos<BitshiftTraits>(input.size()))
      throw exceptions::unexpected_eof_error{"unexpected end of encoded input"};
    return input.size();
  }
};

template <typename EncodingTraits>
class input_sanitizer<EncodingTraits, base_n::padding_policy::optional>
{
public:
  template <typename Input>
  static mgs::ssize_t sanitize(Input const& input)
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const invalid_byte_it = find_invalid_byte<EncodingTraits>(input);
    if (invalid_byte_it != input.end())
    {
      sanitize_invalid_input<EncodingTraits>(input, invalid_byte_it);
      return invalid_byte_it - input.begin();
    }
    // unpadded input, act as if there was padding to detect invalid input
    // (e.g. base64 "Y" -> "Y===")
    else if (invalid_padding_character_pos<BitshiftTraits>(input.size()))
      throw exceptions::unexpected_eof_error{"unexpected end of encoded input"};
    return input.size();
  }
};

template <typename EncodingTraits>
class input_sanitizer<EncodingTraits, base_n::padding_policy::required>
{
  using BitshiftTraits = bitshift_traits<EncodingTraits>;

public:
  template <typename Input>
  static mgs::ssize_t sanitize(Input const& input)
  {
    if (input.size() % BitshiftTraits::nb_encoded_bytes != 0)
      throw exceptions::unexpected_eof_error{"unexpected end of encoded input"};

    auto const invalid_byte_it = find_invalid_byte<EncodingTraits>(input);
    if (invalid_byte_it != input.end())
    {
      sanitize_invalid_input<EncodingTraits>(input, invalid_byte_it);
      return invalid_byte_it - input.begin();
    }
    return input.size();
  }
};
}
}
