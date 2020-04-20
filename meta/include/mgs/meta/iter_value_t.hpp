#pragma once

#include <mgs/meta/concepts/detail/iterator_traits.hpp>
#include <mgs/meta/detected/types/value_type.hpp>
#include <mgs/meta/iterator_traits.hpp>
#include <mgs/meta/readable_traits.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct iter_value : meta::readable_traits<T>
{
};

template <typename T>
struct iter_value<
    T,
    std::enable_if_t<
        !is_iterator_traits_primary_template<meta::iterator_traits<T>>::value &&
        meta::is_detected<meta::detected::types::value_type, T>::value>>
{
  using value_type = meta::detected::types::value_type<T>;
};
}

namespace meta
{
template <typename T>
using iter_value_t = typename detail::iter_value<T>::value_type;
}
}
