#pragma once

#include <tuple>
#include <type_traits>

#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/detected/static_member_functions/create.hpp>
#include <mgs/codecs/output_traits_fwd.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace codecs
{
template <typename T, typename IS>
struct is_codec_output
{
private:
  static constexpr auto const has_create_method =
      meta::is_detected_exact<T,
                              detected::static_member_functions::create,
                              meta::detected_t<codecs::output_traits, T>,
                              IS>::value;

public:
  using requirements = std::tuple<codecs::is_input_source<IS>>;

  static constexpr auto const value =
      codecs::is_input_source<IS>::value && has_create_method;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model codecs::codec_output");
    static_assert(has_create_method,
                  "invalid or missing static member function: 'T "
                  "mgs::codecs::output_traits<T>::create(IS)'");
    return 1;
  }
};

template <typename T,
          typename IS,
          typename = std::enable_if_t<is_codec_output<T, IS>::value>>
using codec_output = T;
}
}
