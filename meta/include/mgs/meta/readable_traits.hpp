#pragma once

#include <type_traits>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/element_type.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct cond_value_type
{
};

template <typename T>
struct cond_value_type<T, std::enable_if_t<std::is_object<T>::value>>
{
  using value_type = std::remove_cv_t<T>;
};

template <typename T, typename = void>
struct readable_traits_impl
{
};

template <typename T>
struct readable_traits_impl<T*> : cond_value_type<T>
{
};

template <typename T>
struct readable_traits_impl<T, std::enable_if_t<std::is_array<T>::value>>
  : readable_traits_impl<std::remove_cv_t<std::remove_extent_t<T>>>
{
};

template <typename T>
struct readable_traits_impl<T const> : readable_traits_impl<T>
{
};

template <typename T>
struct readable_traits_impl<
    T,
    std::enable_if_t<
        meta::is_detected<meta::detected::types::value_type, T>::value>>
  : cond_value_type<meta::detected_t<meta::detected::types::value_type, T>>
{
};

template <typename T>
struct readable_traits_impl<
    T,
    std::enable_if_t<
        meta::is_detected<meta::detected::types::element_type, T>::value>>
  : cond_value_type<meta::detected_t<meta::detected::types::element_type, T>>
{
};
}

namespace meta
{
template <typename T>
struct readable_traits : detail::readable_traits_impl<T>
{
};
}
}
