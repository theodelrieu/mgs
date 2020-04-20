#pragma once

#include <limits>
#include <tuple>
#include <type_traits>

#include <mgs/config.hpp>
#include <mgs/meta/concepts/integral.hpp>
#include <mgs/meta/concepts/same_as.hpp>

#if MGS_HAS_CPP17
#include <cstddef> 
#endif

namespace mgs
{
namespace codecs
{
template <typename T>
struct is_byte_type
{
  using requirements = std::tuple<>;

  static constexpr auto const value =
#if MGS_HAS_CPP17
      meta::is_same_as<std::byte, T>::value ||
#endif
      (meta::is_integral<T>::value &&
       std::numeric_limits<T>::digits + std::numeric_limits<T>::is_signed ==
           std::numeric_limits<unsigned char>::digits);

  static constexpr int trigger_static_asserts()
  {
    static_assert(value,
                  "T does not model codecs::byte_type. It does not model "
                  "meta::integral or does not represent the same number "
                  "of bits as unsigned char");
    return 1;
  }
};

template <typename T>
constexpr auto is_byte_type_v = is_byte_type<T>::value;

template <typename T, typename = std::enable_if_t<is_byte_type_v<T>>>
using byte_type = T;
}
}
