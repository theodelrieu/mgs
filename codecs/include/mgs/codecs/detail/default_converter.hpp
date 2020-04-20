#pragma once

#include <array>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include <mgs/config.hpp>

#include <mgs/codecs/basic_input_range.hpp>
#include <mgs/codecs/concepts/input_source.hpp>
#include <mgs/codecs/concepts/sized_input_source.hpp>
#include <mgs/codecs/detail/read_at_most.hpp>
#include <mgs/exceptions/unexpected_eof_error.hpp>
#include <mgs/meta/concepts/copyable.hpp>
#include <mgs/meta/concepts/default_constructible.hpp>
#include <mgs/meta/concepts/movable.hpp>
#include <mgs/meta/concepts/output_iterator.hpp>
#include <mgs/meta/concepts/output_range.hpp>
#include <mgs/meta/detected.hpp>
#include <mgs/meta/detected/member_functions/resize.hpp>
#include <mgs/meta/detected/types/key_type.hpp>
#include <mgs/meta/detected/types/size_type.hpp>
#include <mgs/meta/iterator_t.hpp>
#include <mgs/meta/priority_tag.hpp>
#include <mgs/ssize_t.hpp>

namespace mgs
{
namespace detail
{
template <typename ResizableOutputRange, typename T>
ResizableOutputRange fill_resizable_output_range(
    codecs::sized_input_source<T, meta::iterator_t<ResizableOutputRange>>& is,
    meta::priority_tag<1>)
{
  using std::begin;
  using size_type = typename ResizableOutputRange::size_type;

  // use max to force a read when max_remaining_size returns 0
  auto const max_size = is.max_remaining_size();
  ResizableOutputRange ret(max_size, 0);

  auto to_read = std::max<mgs::ssize_t>(max_size, 1);
  auto it = begin(ret);
  auto total_read = size_type{0};

  for (auto res = is.read(it, to_read); res.second != 0;
       res = is.read(it, to_read))
  {
    it = std::move(res.first);
    total_read += res.second;
  }

  ret.resize(total_read);
  return ret;
}

template <typename ResizableOutputRange, typename T>
ResizableOutputRange fill_resizable_output_range(
    codecs::input_source<T, meta::iterator_t<ResizableOutputRange>>& is,
    meta::priority_tag<0>)
{
  using std::begin;
  using size_type = typename ResizableOutputRange::size_type;

  constexpr auto block_size = 256;

  ResizableOutputRange ret(block_size, 0);

  auto total_read = size_type{0};
  auto it = begin(ret);
  for (auto res = is.read(it, block_size); res.second != 0;
       res = is.read(it, block_size))
  {
    it = std::move(res.first);
    total_read += res.second;
    ret.resize(total_read + block_size);
  }
  ret.resize(total_read);
  return ret;
}

template <typename Output, typename = void>
struct default_converter
{
private:
  // Overload for containers which have the following properties:
  // - meta::default_constructible
  // - meta::copyable or meta::movable
  // - Output::resize(Output::size_type)
  template <typename T,
            typename R = meta::output_range<Output, typename T::element_type>,
            typename SizeType = typename R::size_type,
            typename = std::enable_if_t<
                meta::is_default_constructible<R>::value &&
                // Keep those in C++17 as well, fill_resizable_container
                // relies on NRVO, not on Guaranteed Copy Elision.
                (meta::is_copyable<R>::value || meta::is_movable<R>::value) &&
                meta::is_detected<meta::detected::member_functions::resize,
                                  R&,
                                  SizeType>::value>>
  static R create_impl(T& is, meta::priority_tag<1>)
  {
    return fill_resizable_output_range<R>(is, meta::priority_tag<1>{});
  }

  // Fallback overload, non-associative containers which:
  // - can be constructed with an Iterator range
  // - are copy or move constructible (pre C++17)
  template <typename T,
            typename = codecs::input_source<T>,
            typename R = Output,
            typename Iterator = meta::iterator_t<codecs::basic_input_range<T>>,
            typename = std::enable_if_t<
  // Guaranteed copy-elision
#if MGS_HAS_CPP17
                (meta::is_move_constructible<R>::value ||
                 meta::is_copy_constructible<R>::value) &&
#endif
                meta::is_constructible_from<R, Iterator, Iterator>::value &&
                // Associative containers' iterator-range constructors are not
                // SFINAE-friendly...
                !meta::is_detected<meta::detected::types::key_type, R>::value>>
  static R create_impl(T is, meta::priority_tag<0>)
  {
    auto input_range = codecs::make_input_range(is);
    return R(input_range.begin(), input_range.end());
  }

public:
  template <typename T>
  static auto create(codecs::input_source<T> is)
      -> decltype(create_impl(std::move(is), meta::priority_tag<1>{}))
  {
    return create_impl(is, meta::priority_tag<1>{});
  }
};

template <typename C, std::size_t N>
struct default_converter<std::array<C, N>>
{
  template <typename T>
  static std::array<C, N> create(codecs::input_source<T, C*> is)
  {
    std::array<C, N> ret;

    auto const res = detail::read_at_most(is, ret.data(), N);
    if (res.second < static_cast<mgs::ssize_t>(N))
      throw exceptions::unexpected_eof_error("output buffer is too large");
    if (detail::read_at_most(is, ret.data(), 1).second != 0)
      throw exceptions::unexpected_eof_error("output buffer is too small");
    return ret;
  }
};
}
}
