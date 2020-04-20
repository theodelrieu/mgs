#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/codecs/concepts/codec.hpp>
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
  static IS make_encoder(IS);

  template <typename IS>
  static IS make_decoder(IS);
};

struct valid_codec
{
  using traits = valid_traits;

  template <typename T = traits::default_encoded_output, typename IS>
  static T encode(IS);

  template <typename T = traits::default_decoded_output, typename IS>
  static T decode(IS);
};
}

TEST_CASE("codecs::codec")
{
  static_assert(is_codec<valid_codec>::value, "");

  static_assert(!is_codec<void>::value, "");
  static_assert(!is_codec<struct incomplete*>::value, "");

  static_assert(is_codec<valid_codec,
                         std::string,
                         std::string,
                         valid_input_source,
                         valid_input_source>::value,
                "");
}
