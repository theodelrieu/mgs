#pragma once

#include <type_traits>
#include <iterator>

#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
// Before C++20, those three iterators define difference_type as void
// and thus they do not model weakly_incrementable.
// They do not model input_or_output_iterator either, because they are not default_constructible
template <typename T, typename = void>
struct is_insert_iterator : std::false_type
{
};

template <typename T>
struct is_insert_iterator<
    std::back_insert_iterator<T>,
    // instantiate the iterator to ensure T is not some
    // weird type (e.g. void)
    meta::void_t<typename std::back_insert_iterator<T>::container_type>>
  : std::true_type
{
};

template <typename T>
struct is_insert_iterator<
    std::front_insert_iterator<T>,
    meta::void_t<typename std::front_insert_iterator<T>::container_type>>
  : std::true_type
{
};

template <typename T>
struct is_insert_iterator<
    std::insert_iterator<T>,
    meta::void_t<typename std::insert_iterator<T>::container_type>>
  : std::true_type
{
};
}
}
