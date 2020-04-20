#pragma once

#include <string>
#include <utility>
#include <vector>

#include <mgs/base_n/decode_algorithm.hpp>
#include <mgs/base_n/encode_algorithm.hpp>
#include <mgs/codecs/concepts/byte_type.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/element_type.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/priority_tag.hpp>

namespace mgs
{
namespace base_n
{
template <typename EncodingTraits, typename DecodingTraits = EncodingTraits>
class basic_codec_traits
{
public:
  template <typename IS,
            typename = codecs::byte_type<
                meta::detected_t<meta::detected::types::element_type, IS>>>
  static encode_algorithm<EncodingTraits, IS> make_encoder(
      codecs::input_source<IS> is)
  {
    return base_n::encode_algorithm<EncodingTraits, IS>(std::move(is));
  }

  template <typename IS,
            typename = codecs::byte_type<
                meta::detected_t<meta::detected::types::element_type, IS>>>
  static decode_algorithm<DecodingTraits, IS> make_decoder(
      codecs::input_source<IS> is)
  {
    return base_n::decode_algorithm<DecodingTraits, IS>(std::move(is));
  }

  using default_encoded_output = std::string;
  using default_decoded_output = std::vector<unsigned char>;
};
}
}
