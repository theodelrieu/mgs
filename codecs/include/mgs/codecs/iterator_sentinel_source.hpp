#pragma once

#include <algorithm>
#include <type_traits>
#include <utility>

#include <mgs/codecs/detail/string_literal_char_type.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/input_range.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/sentinel_for.hpp>
#include <mgs/meta/concepts/sized_sentinel_for.hpp>
#include <mgs/meta/iter_value_t.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/sentinel_t.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace codecs
{
template <typename I, typename S = I>
class iterator_sentinel_source
{
  static_assert(meta::is_input_iterator<I>::value,
                "I does not model meta::input_iterator");
  static_assert(meta::is_sentinel_for<S, I>::value,
                "S does not model meta::sentinel_for<I>");

public:
  using element_type = meta::iter_value_t<I>;

  iterator_sentinel_source() = default;
  iterator_sentinel_source(I begin, S end);

  I begin() const;
  S end() const;

  template <typename OutputIterator,
            typename = std::enable_if_t<
                meta::is_output_iterator<OutputIterator, element_type>::value>>
  std::pair<OutputIterator, mgs::ssize_t> read(OutputIterator out,
                                               mgs::ssize_t n);

  template <
      typename S2 = S,
      typename = std::enable_if_t<meta::is_sized_sentinel_for<S2, I>::value>>
  mgs::ssize_t max_remaining_size() const;

private:
  I _current{};
  S _end{};
};

template <typename I, typename S>
auto make_iterator_sentinel_source(meta::input_iterator<I> begin,
                                   meta::sentinel_for<S, I> end)
    -> iterator_sentinel_source<I, S>
{
  return {std::move(begin), std::move(end)};
}

// For some "fun" reasons, trying to disambiguate the following overloads with
// priority_tag results in the following warning: ISO C++ says that these are
// ambiguous, even though the worst conversion for the first is better than the
// worst conversion for the second.

template <typename R, typename = std::enable_if_t<!std::is_array<R>::value>>
auto make_iterator_sentinel_source(meta::input_range<R>& rng)
    -> iterator_sentinel_source<meta::iterator_t<R>, meta::sentinel_t<R>>
{
  using std::begin;
  using std::end;

  return {begin(rng), end(rng)};
}

template <typename R, typename = std::enable_if_t<!std::is_array<R>::value>>
auto make_iterator_sentinel_source(meta::input_range<R const> & rng)
    -> iterator_sentinel_source<meta::iterator_t<R const>, meta::sentinel_t<R const>>
{
  using std::begin;
  using std::end;

  return {begin(rng), end(rng)};
}

template <typename CharT,
          std::size_t N,
          std::enable_if_t<!detail::is_string_literal_char_type<CharT>::value,
                           int> = 0>
auto make_iterator_sentinel_source(CharT const (&tab)[N])
    -> iterator_sentinel_source<CharT const*>
{
  auto const b = std::begin(tab);
  auto const e = std::end(tab);

  return {b, e};
}

template <typename CharT,
          std::size_t N,
          std::enable_if_t<detail::is_string_literal_char_type<CharT>::value,
                           int> = 0>
auto make_iterator_sentinel_source(CharT const (&tab)[N])
    -> iterator_sentinel_source<CharT const*>
{
  auto const b = std::begin(tab);
  auto const e = std::end(tab);

  return {b, std::find(b, e, CharT{})};
}
}
}

#include <mgs/codecs/detail/iterator_sentinel_source_impl.hpp>
