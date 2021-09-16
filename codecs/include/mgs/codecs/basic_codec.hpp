#pragma once

#include <cstring>
#include <utility>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detected/static_member_functions/make_decoder.hpp>
#include <mgs/codecs/detected/static_member_functions/make_encoder.hpp>
#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/priority_tag.hpp>
#include <mgs/meta/sentinel_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename CodecTraits>
class basic_codec
{
public:
  using traits = CodecTraits;

  using default_encoded_output = typename traits::default_encoded_output;
  using default_decoded_output = typename traits::default_decoded_output;

private:
  template <typename IS>
  static auto make_encoder_impl(codecs::input_source<IS> is,
                                meta::priority_tag<3>)
      -> decltype(traits::make_encoder(std::move(is)))
  {
    return traits::make_encoder(std::move(is));
  }

  template <typename I, typename S>
  static auto make_encoder_impl(meta::input_iterator<I> i,
                                meta::sentinel_for<S, I> s,
                                meta::priority_tag<2>)
      -> decltype(
          traits::make_encoder(std::declval<iterator_sentinel_source<I, S>>()))
  {
    auto is = codecs::make_iterator_sentinel_source(std::move(i), std::move(s));
    return traits::make_encoder(std::move(is));
  }

  template <typename InputRange>
  static auto make_encoder_impl(meta::input_range<InputRange>& r,
                                meta::priority_tag<1>)
      -> decltype(traits::make_encoder(make_iterator_sentinel_source(r)))
  {
    return traits::make_encoder(make_iterator_sentinel_source(r));
  }

  template <typename InputRange>
  static auto make_encoder_impl(meta::input_range<InputRange> const& r,
                                meta::priority_tag<0>)
      -> decltype(traits::make_encoder(make_iterator_sentinel_source(r)))
  {
    return traits::make_encoder(make_iterator_sentinel_source(r));
  }

  template <typename IS>
  static auto make_decoder_impl(codecs::input_source<IS> is,
                                meta::priority_tag<3>)
      -> decltype(traits::make_decoder(std::move(is)))
  {
    return traits::make_decoder(std::move(is));
  }

  template <typename I, typename S>
  static auto make_decoder_impl(meta::input_iterator<I> i,
                                meta::sentinel_for<S, I> s,
                                meta::priority_tag<2>)
      -> decltype(
          traits::make_decoder(std::declval<iterator_sentinel_source<I, S>>()))
  {
    auto is = codecs::make_iterator_sentinel_source(std::move(i), std::move(s));
    return traits::make_decoder(std::move(is));
  }

  template <typename InputRange>
  static auto make_decoder_impl(meta::input_range<InputRange>& r,
                                meta::priority_tag<1>)
      -> decltype(traits::make_decoder(make_iterator_sentinel_source(r)))
  {
    return traits::make_decoder(make_iterator_sentinel_source(r));
  }

  template <typename InputRange>
  static auto make_decoder_impl(meta::input_range<InputRange> const& r,
                                meta::priority_tag<0>)
      -> decltype(traits::make_decoder(make_iterator_sentinel_source(r)))
  {
    return traits::make_decoder(make_iterator_sentinel_source(r));
  }

public:
  template <typename... Args>
  static auto make_encoder(Args&&... args)
      -> decltype(make_encoder_impl(std::forward<Args>(args)...,
                                    meta::priority_tag<4>{}))
  {
    return make_encoder_impl(std::forward<Args>(args)...,
                             meta::priority_tag<4>{});
  }

  template <typename... Args>
  static auto make_decoder(Args&&... args)
      -> decltype(make_decoder_impl(std::forward<Args>(args)...,
                                    meta::priority_tag<4>{}))
  {
    return make_decoder_impl(std::forward<Args>(args)...,
                             meta::priority_tag<4>{});
  }

  template <typename T = default_encoded_output, typename... Args>
  static auto encode(Args&&... args) -> codecs::codec_output<
      decltype(
          output_traits<T>::create(make_encoder(std::forward<Args>(args)...))),
      decltype(make_encoder(std::forward<Args>(args)...))>
  {
    return output_traits<T>::create(make_encoder(std::forward<Args>(args)...));
  }

  template <typename T = default_decoded_output, typename... Args>
  static auto decode(Args&&... args) -> codecs::codec_output<
      decltype(
          output_traits<T>::create(make_decoder(std::forward<Args>(args)...))),
      decltype(make_decoder(std::forward<Args>(args)...))>
  {
    return output_traits<T>::create(make_decoder(std::forward<Args>(args)...));
  }

  template <typename T = default_encoded_output>
  static auto lazy_encode()
  {
    return
        [](auto&&... args)
            -> codecs::codec_output<
                decltype(output_traits<T>::create(
                    make_encoder(std::forward<decltype(args)>(args)...))),
                decltype(make_encoder(std::forward<decltype(args)>(args)...))> {
          return output_traits<T>::create(
              make_encoder(std::forward<decltype(args)>(args)...));
        };
  }

  template <typename T = default_decoded_output>
  static auto lazy_decode()
  {
    return
        [](auto&&... args)
            -> codecs::codec_output<
                decltype(output_traits<T>::create(
                    make_decoder(std::forward<decltype(args)>(args)...))),
                decltype(make_decoder(std::forward<decltype(args)>(args)...))> {
          return output_traits<T>::create(
              make_decoder(std::forward<decltype(args)>(args)...));
        };
  }
};
}
}
