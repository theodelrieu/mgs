#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <limits>

#include <mgs/base_n/concepts/encoding_traits.hpp>
#include <mgs/base_n/detail/bitset_utils.hpp>
#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/detail/encoded_input_reader.hpp>
#include <mgs/base_n/detail/input_sanitizer.hpp>
#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/detail/max_decoded_size.hpp>
#include <mgs/base_n/detail/output_decoder.hpp>
#include <mgs/base_n/detail/span.hpp>
#include <mgs/base_n/detail/static_vector.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>
#include <mgs/codecs/detail/read_at_most.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace base_n
{
template <typename Traits, typename IS>
class decode_algorithm
{
private:
  static_assert(base_n::is_encoding_traits<Traits>::value,
                "Traits does not model base_n::encoding_traits");
  static_assert(codecs::is_input_source<IS, unsigned char*>::value,
                "IS does not model codecs::input_source<unsigned char*>");

  using BitshiftTraits = detail::bitshift_traits<Traits>;

  static constexpr std::size_t nb_bytes_to_read =
      (256 / BitshiftTraits::nb_encoded_bytes) *
      BitshiftTraits::nb_encoded_bytes;
  static_assert(nb_bytes_to_read % BitshiftTraits::nb_encoded_bytes == 0,
                "The impossible has occurred");

  using buffer_type = detail::static_vector<unsigned char, 256>;

public:
  using element_type = unsigned char;

  explicit decode_algorithm(IS is) : _input_source(std::move(is))
  {
  }

  template <typename T = IS, typename = codecs::sized_input_source<T>>
  mgs::ssize_t max_remaining_size() const
  {
    auto const max_remaining = _input_source.max_remaining_size();
    auto s = detail::max_decoded_size<Traits>{}(max_remaining);
    if (s == -1)
    {
      // we must round, since this is a maximum, and not the exact size
      // not doing so would result in a failure in some cases
      // e.g. when decoding a decoder
      auto const rounded =
          max_remaining - (max_remaining % BitshiftTraits::nb_encoded_bytes);
      s = detail::max_decoded_size<Traits>{}(rounded);
    }
    return (_buffer.size() - _index) + s;
  }

  template <typename O>
  std::pair<O, mgs::ssize_t> read(meta::output_iterator<O, element_type> o,
                                  mgs::ssize_t n)
  {
    if (_buffer.size() == _index)
    {
      read_input_source();
      _index = 0;
    }
    auto const to_read = std::min<mgs::ssize_t>(_buffer.size() - _index, n);
    o = std::copy_n(_buffer.data() + _index, to_read, o);
    _index += to_read;
    return std::make_pair(std::move(o), to_read);
  }

private:
  auto read_input_source_impl()
  {
    detail::static_vector<unsigned char, nb_bytes_to_read> ret;
    ret.resize(nb_bytes_to_read);
    auto const res =
        detail::read_at_most(_input_source, ret.data(), nb_bytes_to_read);
    ret.resize(res.second);
    auto const sanitized_size = detail::input_sanitizer<Traits>::sanitize(ret);
    if (sanitized_size != ret.size())
    {
      unsigned char dummy;
      // input left, but we found padding last time => invalid input
      if (_input_source.read(&dummy, 1).second != 0)
      {
        throw exceptions::invalid_input_error{
            "more input was found after padding"};
      }
    }
    ret.resize(sanitized_size);
    return ret;
  }

  void read_input_source()
  {
    auto const input = read_input_source_impl();

    auto const nb_loop_iterations =
        std::lldiv(input.size(), BitshiftTraits::nb_encoded_bytes);

    _buffer.resize(static_cast<mgs::ssize_t>(nb_loop_iterations.quot *
                                             BitshiftTraits::nb_decoded_bytes));
    for (auto i = 0; i < nb_loop_iterations.quot; ++i)
    {
      auto const indices_bits = detail::indices_to_bitset<Traits>(
          input.begin() + (i * BitshiftTraits::nb_encoded_bytes),
          BitshiftTraits::nb_encoded_bytes);

      detail::output_decoder<Traits>::decode(
          indices_bits,
          _buffer.begin() + (i * BitshiftTraits::nb_decoded_bytes));
    }
    if (nb_loop_iterations.rem)
    {
      auto const nb_last_bytes_to_decode = static_cast<mgs::ssize_t>(
          (nb_loop_iterations.rem * BitshiftTraits::nb_index_bits) / 8);
      auto const old_size = _buffer.size();
      _buffer.resize(old_size + nb_last_bytes_to_decode);

      auto const indices_bits = detail::indices_to_bitset<Traits>(
          input.begin() +
              (nb_loop_iterations.quot * BitshiftTraits::nb_encoded_bytes),
          static_cast<mgs::ssize_t>(nb_loop_iterations.rem));

      detail::output_decoder<Traits>::decode(
          indices_bits, _buffer.begin() + old_size, nb_last_bytes_to_decode);
    }
  }

  IS _input_source;
  buffer_type _buffer;
  mgs::ssize_t _index{};
};

template <typename Traits, typename IS>
constexpr std::size_t decode_algorithm<Traits, IS>::nb_bytes_to_read;
}
}
