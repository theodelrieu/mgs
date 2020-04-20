#pragma once

#include <mgs/meta/concepts/detail/iterator_traits.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/incrementable_traits.hpp>
#include <mgs/meta/iterator_traits.hpp>

namespace mgs
{
namespace detail
{
template <typename T, typename = void>
struct iter_difference : meta::incrementable_traits<T>
{
};

template <typename T>
struct iter_difference<
    T,
    std::enable_if_t<
        !is_iterator_traits_primary_template<meta::iterator_traits<T>>::value &&
        meta::is_detected<meta::detected::types::difference_type, T>::value>>
{
  using difference_type = meta::detected::types::difference_type<T>;
};
}

namespace meta
{
template <typename T>
using iter_difference_t = typename detail::iter_difference<T>::difference_type;
}
}
