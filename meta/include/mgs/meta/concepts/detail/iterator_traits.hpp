#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

#include <mgs/meta/concepts/complete_type.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/pointer.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/iterator_traits.hpp>

namespace mgs
{
namespace detail
{
template <typename T>
struct is_iterator_traits_primary_template : std::false_type
{
};

template <typename T>
struct is_iterator_traits_primary_template<meta::iterator_traits<T>>
{
  using traits = meta::iterator_traits<T>;

  static constexpr auto const has_value_type =
      meta::is_detected<meta::detected::types::value_type, traits>::value;

  static constexpr auto const has_difference_type =
      meta::is_detected<meta::detected::types::difference_type, traits>::value;

  static constexpr auto const has_pointer =
      meta::is_detected<meta::detected::types::pointer, traits>::value;

  static constexpr auto const has_iterator_category =
      meta::is_detected<meta::detected::types::iterator_category,
                        traits>::value;

  static constexpr auto const has_reference =
      meta::is_detected<meta::detected::types::reference, traits>::value;

  static constexpr auto const value = !has_pointer && !has_reference &&
                                      !has_value_type && !has_difference_type &&
                                      !has_iterator_category;
};

template <typename T>
struct is_iterator_traits_primary_template<std::iterator_traits<T>>
  : meta::iterator_traits<T>
{
};

template <typename T>
struct is_iterator_traits_impl : std::false_type
{
  static constexpr int trigger_static_asserts()
  {
    return 1;
  }
};

template <typename T>
struct is_iterator_traits_impl<meta::iterator_traits<T>>
{
private:
  using traits = meta::iterator_traits<T>;

  static constexpr auto const has_value_type =
      meta::is_detected<meta::detected::types::value_type, traits>::value;

  static constexpr auto const has_difference_type =
      meta::is_detected<meta::detected::types::difference_type, traits>::value;

  static constexpr auto const has_pointer =
      meta::is_detected<meta::detected::types::pointer, traits>::value;

  static constexpr auto const has_iterator_category =
      meta::is_detected<meta::detected::types::iterator_category,
                        traits>::value;

  static constexpr auto const has_reference =
      meta::is_detected<meta::detected::types::reference, traits>::value;

public:
  static constexpr auto value = has_value_type && has_difference_type &&
                                has_pointer && has_iterator_category &&
                                has_reference;

  static constexpr int trigger_static_asserts()
  {
    static_assert(has_value_type,
                  "Missing type alias 'std::iterator_traits<T>::value_type'");
    static_assert(
        has_difference_type,
        "Missing type alias 'std::iterator_traits<T>::difference_type'");
    static_assert(has_pointer,
                  "Missing type alias 'std::iterator_traits<T>::pointer'");
    static_assert(
        has_iterator_category,
        "Missing type alias 'std::iterator_traits<T>::iterator_category'");
    static_assert(has_reference,
                  "Missing type alias 'std::iterator_traits<T>::reference'");
    return 1;
  }
};

template <typename T>
struct is_iterator_traits_impl<std::iterator_traits<T>>
  : is_iterator_traits_impl<meta::iterator_traits<T>>
{
};

template <typename T>
struct is_iterator_traits : is_iterator_traits_impl<T>
{
  using requirements = std::tuple<>;

  static constexpr int trigger_static_asserts()
  {
    static_assert(is_iterator_traits::value, "T is not a valid iterator_traits type");
    return is_iterator_traits_impl<T>::trigger_static_asserts();
  }
};

template <typename T>
constexpr auto is_iterator_traits_v = is_iterator_traits<T>::value;
}
}
