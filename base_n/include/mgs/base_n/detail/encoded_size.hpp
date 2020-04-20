#pragma once

#include <cstddef>

#include <mgs/base_n/detail/bitshift_traits.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename EncodingTraits,
          base_n::padding_policy = EncodingTraits::padding_policy>
struct encoded_size;

template <typename EncodingTraits>
struct encoded_size<EncodingTraits, base_n::padding_policy::required>
{
  constexpr mgs::ssize_t operator()(mgs::ssize_t decoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const quot = decoded_size / BitshiftTraits::nb_decoded_bytes;
    auto const rem = decoded_size % BitshiftTraits::nb_decoded_bytes;

    return quot * BitshiftTraits::nb_encoded_bytes +
           int(rem > 0) * BitshiftTraits::nb_encoded_bytes;
  }
};

template <typename EncodingTraits>
struct encoded_size<EncodingTraits, base_n::padding_policy::none>
{
  constexpr mgs::ssize_t operator()(mgs::ssize_t decoded_size) const
  {
    using BitshiftTraits = bitshift_traits<EncodingTraits>;

    auto const quot = (decoded_size * BitshiftTraits::nb_encoded_bytes) /
                      BitshiftTraits::nb_decoded_bytes;
    auto const rem = (decoded_size * BitshiftTraits::nb_encoded_bytes) %
                     BitshiftTraits::nb_decoded_bytes;

    return quot + int(rem > 0);
  }
};

// optional does not make sense when encoding.
template <typename EncodingTraits>
struct encoded_size<EncodingTraits, base_n::padding_policy::optional>;
}
}
