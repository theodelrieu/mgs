#include <forward_list>
#include <list>
#include <string>
#include <sstream>
#include <tuple>

#include <catch2/catch.hpp>

#include <mgs/codecs/basic_input_range.hpp>
#include <mgs/meta/concepts/input_iterator.hpp>
#include <mgs/meta/concepts/input_or_output_iterator.hpp>

using namespace mgs;
using namespace std::string_literals;

namespace
{
class noop_source
{
public:
  using element_type = char;

  explicit noop_source(std::string s) : _s(s)
  {
  }

  template <typename O>
  std::pair<O, mgs::ssize_t> read(O o, mgs::ssize_t n)
  {
    auto const to_read = std::min<mgs::ssize_t>(n, _s.size() - _idx);
    o = std::copy_n(_s.data() + _idx, to_read, o);
    _idx += to_read;
    return std::make_pair(std::move(o), to_read);
  }

private:
  std::string _s;
  mgs::ssize_t mutable _idx{0};
};
}

TEST_CASE("basic_input_range_iterator")
{
  SECTION("regular tests")
  {
    noop_source source("abcdefghijklmnopqrstuvwxyz");

    auto enc = codecs::make_input_range(source);
    auto current = enc.begin();
    auto end = enc.end();

    static_assert(meta::is_input_or_output_iterator<decltype(current)>::value,
                  "");
    static_assert(
        meta::is_sentinel_for<decltype(end), decltype(current)>::value, "");
    using encoder_tag = meta::detected::types::iterator_category<
        std::iterator_traits<decltype(current)>>;

    static_assert(meta::is_input_iterator<decltype(current)>::value, "");
    CHECK(*current == *current);
    CHECK(*current == 'a');
    CHECK(*++current == 'b');
    current++;
    CHECK(*current == 'c');
    CHECK(current != end);
    CHECK(current == current);
    CHECK(std::distance(current, end) == 24);
  }

  SECTION("empty range")
  {
    noop_source empty_source("");

    auto enc = codecs::make_input_range(empty_source);

    auto current = enc.begin();
    auto end = enc.end();

    CHECK(current == current);
    CHECK(current == end);
    CHECK(std::distance(current, end) == 0);
  }
}
