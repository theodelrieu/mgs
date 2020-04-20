#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/codecs/concepts/codec_traits.hpp>
#include <mgs/codecs/output_traits.hpp>

using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct valid_input_source
{
  using element_type = char;

  std::pair<char*, int>  read(char*, int);
};

struct valid_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename IS>
  static valid_input_source make_encoder(IS);

  template <typename IS>
  static valid_input_source make_decoder(IS);
};

struct valid_traits_custom_default_types
{
  using default_encoded_output = int;
  using default_decoded_output = int;

  template <typename IS>
  static valid_input_source make_encoder(IS);

  template <typename IS>
  static valid_input_source make_decoder(IS);
};

struct no_encoded_output_traits
{
  using default_decoded_output = std::string;

  template <typename IS>
  static valid_input_source make_encoder(IS);

  template <typename IS>
  static valid_input_source make_decoder(IS);
};

struct no_decoded_output_traits
{
  using default_encoded_output = std::string;

  template <typename IS>
  static valid_input_source make_encoder(IS);

  template <typename IS>
  static valid_input_source make_decoder(IS);
};

struct no_make_encoder_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename IS>
  static valid_input_source make_decoder(IS);
};

struct no_make_decoder_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename IS>
  static valid_input_source make_encoder(IS);
};

struct invalid_return_traits
{
  using default_encoded_output = std::string;
  using default_decoded_output = std::string;

  template <typename IS>
  static int make_encoder(IS);

  template <typename IS>
  static valid_input_source make_decoder(IS);
};
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<int>
{
  template <typename U>
  static int create(U);
};
}
}

TEST_CASE("CodecTraits")
{
  static_assert(is_codec_traits<valid_traits>::value, "");
  static_assert(is_codec_traits<valid_traits_custom_default_types,
                                valid_input_source,
                                valid_input_source>::value,
                "");

  static_assert(!is_codec_traits<no_encoded_output_traits>::value, "");
  static_assert(!is_codec_traits<no_decoded_output_traits>::value, "");
  static_assert(!is_codec_traits<no_make_encoder_traits>::value, "");
  static_assert(!is_codec_traits<no_make_decoder_traits>::value, "");
  static_assert(!is_codec_traits<invalid_return_traits>::value, "");
}
