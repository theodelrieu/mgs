#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <mgs/base_n/detail/math.hpp>
#include <mgs/base_n/detected/static_data_members/alphabet.hpp>
#include <mgs/base_n/detected/static_data_members/padding_character.hpp>
#include <mgs/base_n/detected/static_data_members/padding_policy.hpp>
#include <mgs/base_n/detected/static_member_functions/index_of.hpp>
#include <mgs/base_n/padding_policy.hpp>
#include <mgs/meta/detected.hpp>

namespace mgs
{
namespace detail
{
template <typename T, bool Cond1 = false, bool Cond2 = false>
struct is_encoding_traits_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    static_assert(Cond1,
                  "invalid or missing static data member: 'T::padding_policy'");
    static_assert(Cond1,
                  "invalid or missing static data member: 'T::alphabet'");
    return 1;
  }
};

template <typename T>
struct is_encoding_traits_impl<T, true, true>
{
private:
  using alphabet_t = base_n::detected::static_data_members::alphabet<T>;

  static constexpr auto const is_alphabet =
      std::is_array<alphabet_t>::value &&
      std::is_same<char const, std::remove_extent_t<alphabet_t>>::value;

  static constexpr auto const is_valid_alphabet_size =
      detail::is_power_of_2<sizeof(alphabet_t)>();

  static constexpr auto const is_valid_padding_character =
      meta::is_detected_convertible<
          char,
          base_n::detected::static_data_members::padding_character,
          T>::value;

  static constexpr auto const has_index_of = meta::is_detected_exact<
      int,
      base_n::detected::static_member_functions::index_of,
      T,
      char>::value;

public:
  using requirements = std::tuple<>;

  static constexpr auto const value =
      is_alphabet && is_valid_alphabet_size &&
      (is_valid_padding_character ||
       T::padding_policy == base_n::padding_policy::none) &&
      has_index_of;

  static constexpr int trigger_static_asserts()
  {
    static_assert(value, "T does not model base_n::encoding_traits");

    static_assert(is_alphabet, "T::alphabet must be a 'char const[]'");
    static_assert(is_valid_alphabet_size,
                  "sizeof(T::alphabet) must be a power of 2");
    static_assert(is_valid_padding_character ||
                      T::padding_policy == base_n::padding_policy::none,
                  "T::padding_character must be convertible to char when "
                  "T::padding_policy is not padding_policy::none");
    static_assert(has_index_of,
                  "invalid or missing function: 'int T::has_index_of(char)'");
    return 1;
  }
};
}

namespace base_n
{
template <typename T>
struct is_encoding_traits
  : detail::is_encoding_traits_impl<
        T,
        meta::is_detected_convertible<
            padding_policy,
            detected::static_data_members::padding_policy,
            T>::value,
        std::is_array<meta::detected_t<detected::static_data_members::alphabet,
                                       T>>::value>
{
  using requirements = std::tuple<>;
};

template <typename T>
constexpr auto is_encoding_traits_v = is_encoding_traits<T>::value;

template <typename T, typename = std::enable_if_t<is_encoding_traits<T>::value>>
using encoding_traits = T;
}
}
