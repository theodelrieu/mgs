#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/codecs/concepts/codec_output.hpp>
#include <mgs/codecs/output_traits.hpp>
#include <mgs/meta/concepts/range.hpp>

using namespace mgs;
using namespace mgs::codecs;

namespace
{
struct valid_input_source
{
  using element_type = char;

  std::pair<char*, int> read(char*, int);
};

struct invalid
{
};

struct valid
{
};
}

namespace mgs
{
namespace codecs
{
template <>
struct output_traits<valid>
{
  template <typename U>
  static valid create(U);
};
}
}

TEST_CASE("codec_output")
{
  static_assert(is_codec_output<std::string, valid_input_source>::value, "");
  static_assert(is_codec_output<valid, valid_input_source>::value, "");

  static_assert(!is_codec_output<invalid, valid_input_source>::value, "");

  static_assert(!is_codec_output<void, valid_input_source>::value, "");
  static_assert(!is_codec_output<void*, valid_input_source>::value, "");
  static_assert(!is_codec_output<struct incomplete, valid_input_source>::value,
                "");
  static_assert(!is_codec_output<struct incomplete*, valid_input_source>::value,
                "");
}
