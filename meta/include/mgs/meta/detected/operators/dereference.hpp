#pragma once

#include <type_traits>
#include <utility>

#include <mgs/meta/detected.hpp>
#include <mgs/meta/void_t.hpp>

namespace mgs
{
namespace detail
{
// Workaround a clang bug with operator*(void*) in unevaluated context
template <typename T, typename = void>
struct dereference_impl
{
};

template <typename T>
struct dereference_impl<T, meta::void_t<decltype(*std::declval<T>())>>
{
  using type = decltype(*std::declval<T>());
};

template <typename T, typename>
struct dereference : dereference_impl<T>
{
};

template <typename T>
struct dereference<T, void*>
{
};
}

namespace meta
{
namespace detected
{
namespace operators
{
template <typename T>
using dereference = typename detail::
    dereference<T, std::remove_cv_t<std::remove_reference_t<T>>>::type;
}
}
}
}
