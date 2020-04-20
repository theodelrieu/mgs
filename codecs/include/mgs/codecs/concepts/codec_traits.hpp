#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detail/detect_input_sources.hpp>
#include <mgs/codecs/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/detected/types/default_decoded_output.hpp>
#include <mgs/codecs/detected/types/default_encoded_output.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename T,
          typename IS1 = meta::detected_t<detail::input_source_1, T>,
          typename IS2 = meta::detected_t<detail::input_source_2, T>>
struct is_codec_traits
{
private:
  using E = meta::detected_t<detected::types::default_encoded_output, T>;
  using D = meta::detected_t<detected::types::default_decoded_output, T>;

  using Encoder =
      meta::detected_t<detected::static_member_functions::make_encoder, T, IS1>;
  using Decoder =
      meta::detected_t<detected::static_member_functions::make_decoder, T, IS2>;

  static constexpr auto const is_encoder =
      codecs::is_input_source<Encoder>::value;
  static constexpr auto const is_decoder =
      codecs::is_input_source<Decoder>::value;

public:
  using requirements = std::tuple<codecs::is_input_source<IS1>,
                                  codecs::is_input_source<IS2>,
                                  codecs::is_codec_output<E, Encoder>,
                                  codecs::is_codec_output<D, Decoder>>;

  static constexpr auto const value =
      codecs::is_input_source<IS1>::value &&
      codecs::is_input_source<IS2>::value && is_encoder && is_decoder &&
      codecs::is_codec_output<E, Encoder>::value &&
      codecs::is_codec_output<D, Decoder>::value;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model codecs::codec_traits");
    static_assert(!std::is_same<meta::nonesuch, IS1>::value,
                  "IS1 default value is invalid. This is most likely due to "
                  "T::default_decoded_output not modelling meta::input_range");
    static_assert(!std::is_same<meta::nonesuch, IS2>::value,
                  "IS2 default value is invalid. This is most likely due to "
                  "T::default_encoded_output not modelling meta::input_range");
    static_assert(is_encoder,
                  "invalid or missing function: 'codecs::input_source "
                  "T::make_encoder(IS1)'");
    static_assert(is_decoder,
                  "invalid or missing function: 'codecs::input_source "
                  "T::make_decoder(IS2)'");
    return 1;
  }
};

template <typename T,
          typename IS1 = meta::detected_t<detail::input_source_1, T>,
          typename IS2 = meta::detected_t<detail::input_source_2, T>>
constexpr auto is_codec_traits_v = is_codec_traits<T, IS1, IS2>::value;

template <typename T,
          typename IS1 = meta::detected_t<detail::input_source_1, T>,
          typename IS2 = meta::detected_t<detail::input_source_2, T>,
          typename = std::enable_if_t<is_codec_traits<T, IS1, IS2>::value>>
using codec_traits = T;
}
}
