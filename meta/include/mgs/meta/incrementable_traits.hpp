#pragma once

#include <cstddef>
#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct workaround_partial_ordering_bug
{
};

template <typename T>
struct workaround_partial_ordering_bug<
    T,
    std::enable_if_t<
        !meta::is_detected<meta::detected::types::difference_type, T>::value &&
        std::is_integral<
            meta::detected_t<meta::detected::operators::substraction,
                             T const&,
                             T const&>>::value>>
{
  using difference_type = std::make_signed_t<
      meta::detected::operators::substraction<T const&, T const&>>;
};

template <typename T, typename = void>
struct incrementable_traits_impl : workaround_partial_ordering_bug<T>
{
};

template <typename T>
struct incrementable_traits_impl<T*, std::enable_if_t<std::is_object<T>::value>>
{
  using difference_type = std::ptrdiff_t;
};

template <typename T>
struct incrementable_traits_impl<T const> : incrementable_traits_impl<T>
{
};

template <typename T>
struct incrementable_traits_impl<
    T,
    std::enable_if_t<
        meta::is_detected<meta::detected::types::difference_type, T>::value>>
{
  using difference_type = meta::detected::types::difference_type<T>;
};
}

namespace meta
{
template <typename T>
struct incrementable_traits : detail::incrementable_traits_impl<T>
{
};
}
}
