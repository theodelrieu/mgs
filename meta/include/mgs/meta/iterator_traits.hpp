#pragma once

#include <iterator>
#include <type_traits>

namespace mgs
{
namespace detail
{
// SFINAE correct version of std::iterator_traits, see:
// https://cplusplus.github.io/LWG/issue2951
template <typename T, typename = void>
struct iterator_traits
{
};

template <typename T>
struct iterator_traits<T, std::enable_if_t<!std::is_pointer<T>::value>>
  : std::iterator_traits<T>
{
};

template <typename T>
struct iterator_traits<T*, std::enable_if_t<std::is_object<T>::value>>
  : std::iterator_traits<T*>
{
};
}

namespace meta
{
template <typename T>
struct iterator_traits : detail::iterator_traits<T>
{
};
}
}
