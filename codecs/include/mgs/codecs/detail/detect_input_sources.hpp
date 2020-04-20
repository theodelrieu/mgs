#pragma once

#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>

namespace mgs
{
namespace detail
{
// hard to SFINAE with a class template, ensure iterator_sentinel_source's requirements
// are fulfilled before evaluation
template <typename T,
          typename D = typename T::default_decoded_output,
          typename I = meta::detected_t<meta::iterator_t, D>,
          typename S = meta::detected_t<meta::sentinel_t, D>,
          typename = std::enable_if_t<meta::is_input_iterator<I>::value &&
                                      meta::is_sentinel_for<S, I>::value>>
using input_source_1 = codecs::iterator_sentinel_source<I, S>;

template <typename T,
          typename E = typename T::default_encoded_output,
          typename I = meta::detected_t<meta::iterator_t, E>,
          typename S = meta::detected_t<meta::sentinel_t, E>,
          typename = std::enable_if_t<meta::is_input_iterator<I>::value &&
                                      meta::is_sentinel_for<S, I>::value>>
using input_source_2 = codecs::iterator_sentinel_source<I, S>;
}
}
