#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/detail/detect_input_sources.hpp>
#include <mgs/codecs/detected/static_member_functions/decode.hpp>
#include <mgs/codecs/detected/static_member_functions/encode.hpp>
#include <mgs/codecs/detected/types/default_decoded_output.hpp>
#include <mgs/codecs/detected/types/default_encoded_output.hpp>
#include <mgs/codecs/detected/types/traits.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename T,
          typename R1 =
              meta::detected_t<detected::types::default_encoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename R2 =
              meta::detected_t<detected::types::default_decoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS1 =
              meta::detected_t<detail::input_source_1,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS2 =
              meta::detected_t<detail::input_source_2,
                               meta::detected_t<detected::types::traits, T>>>
struct is_codec
{
private:
  using IS1_lval = std::add_lvalue_reference_t<IS1>;
  using IS2_lval = std::add_lvalue_reference_t<IS2>;
  using Traits = meta::detected_t<detected::types::traits, T>;

  using DefaultEncodedOutput =
      meta::detected_t<detected::types::default_encoded_output, Traits>;
  using DefaultDecodedOutput =
      meta::detected_t<detected::types::default_decoded_output, Traits>;

  using Encoder = meta::
      detected_t<detected::static_member_functions::make_encoder, Traits, IS1_lval>;

  using Decoder = meta::
      detected_t<detected::static_member_functions::make_decoder, Traits, IS2_lval>;

  using EncodeDefaultResult =
      meta::detected_t<detected::static_member_functions::encode, T, IS1_lval>;

  using EncodeTemplateResult =
      meta::detected_t<detected::static_member_functions::encode_tpl,
                       T,
                       R1,
                       IS1_lval>;

  using DecodeDefaultResult =
      meta::detected_t<detected::static_member_functions::decode, T, IS2_lval>;

  using DecodeTemplateResult =
      meta::detected_t<detected::static_member_functions::decode_tpl,
                       T,
                       R2,
                       IS2_lval>;

  static constexpr auto const has_template_encoded_output =
      is_codec_output<R1, Encoder>::value;

  static constexpr auto const has_template_decoded_output =
      is_codec_output<R2, Decoder>::value;

  static constexpr auto const has_encode_default_result =
      !std::is_same<EncodeDefaultResult, meta::nonesuch>::value &&
      std::is_same<EncodeDefaultResult, DefaultEncodedOutput>::value;

  static constexpr auto const has_encode_template_result =
      !std::is_same<EncodeTemplateResult, meta::nonesuch>::value &&
      std::is_same<EncodeTemplateResult, R1>::value;

  static constexpr auto const has_decode_default_result =
      !std::is_same<DecodeDefaultResult, meta::nonesuch>::value &&
      std::is_same<DecodeDefaultResult, DefaultDecodedOutput>::value;

  static constexpr auto const has_decode_template_result =
      !std::is_same<DecodeTemplateResult, meta::nonesuch>::value &&
      std::is_same<DecodeTemplateResult, R2>::value;

public:
  using requirements = std::tuple<codecs::is_codec_traits<Traits, IS1, IS2>>;

  static constexpr auto const value =
      codecs::is_codec_traits<Traits, IS1, IS2>::value &&
      has_template_encoded_output && has_template_decoded_output &&
      has_encode_default_result && has_encode_template_result &&
      has_decode_default_result && has_decode_template_result;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model codecs::codec");

    static_assert(!std::is_same<meta::nonesuch, IS1>::value,
                  "IS1 default value is invalid. This is most likely due to "
                  "T::default_decoded_output not modelling meta::input_range");
    static_assert(!std::is_same<meta::nonesuch, IS2>::value,
                  "IS2 default value is invalid. This is most likely due to "
                  "T::default_encoded_output not modelling meta::input_range");
    static_assert(has_template_encoded_output,
                  "R1 does not model codecs::codec_output<Encoder>");
    static_assert(has_template_decoded_output,
                  "R2 does not model codecs::codec_output<Decoder>");

    static_assert(has_encode_default_result,
                  "'T::encode(IS1&)' must return T::default_encoded_output");
    static_assert(has_decode_default_result,
                  "'T::decode(IS2&)' must return T::default_decoded_output");
    static_assert(has_encode_template_result,
                  "'T::template encode<R1>(IS1&)' must return R1");
    static_assert(has_decode_template_result,
                  "'T::template decode<R2>(IS2&)' must return R2");
    return 1;
  }
};

template <typename T,
          typename R1 =
              meta::detected_t<detected::types::default_encoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename R2 =
              meta::detected_t<detected::types::default_decoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS1 =
              meta::detected_t<detail::input_source_1,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS2 =
              meta::detected_t<detail::input_source_2,
                               meta::detected_t<detected::types::traits, T>>>
constexpr auto is_codec_v = is_codec<T, R1, R2, IS1, IS2>::value;

template <typename T,
          typename R1 =
              meta::detected_t<detected::types::default_encoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename R2 =
              meta::detected_t<detected::types::default_decoded_output,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS1 =
              meta::detected_t<detail::input_source_1,
                               meta::detected_t<detected::types::traits, T>>,
          typename IS2 =
              meta::detected_t<detail::input_source_2,
                               meta::detected_t<detected::types::traits, T>>,
          typename = std::enable_if_t<is_codec<T, R1, R2, IS1, IS2>::value>>
using codec = T;
}
}
