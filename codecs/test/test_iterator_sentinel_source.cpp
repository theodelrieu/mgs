#include <string>

#include <catch2/catch.hpp>

#include <mgs/codecs/iterator_sentinel_source.hpp>
#include <mgs/config.hpp>
#include <mgs/meta/concepts/range.hpp>
#include <mgs/meta/concepts/semiregular.hpp>

using namespace mgs::codecs;
using namespace mgs::meta;

namespace
{
template <typename Ret = std::string, typename I, typename S>
Ret read_all(iterator_sentinel_source<I, S>& src)
{
  Ret ret;

  while (src.read(std::back_inserter(ret), 4096).second)
    ;
  return ret;
}
}

TEST_CASE("codecs::iterator_sentinel_source")
{
  SECTION("read")
  {
    SECTION("std::string")
    {
      std::string s("abcdefghijklmnopqrstuvwxyz");
      auto is = make_iterator_sentinel_source(s);
      CHECK(read_all(is) == s);
    }

    SECTION("std::vector<char>")
    {
      std::vector<char> v{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_iterator_sentinel_source(v);
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), v.begin(), v.end()));
    }

    SECTION("char []")
    {
      char const tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_iterator_sentinel_source(tab);
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("int []")
    {
      int const tab[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                         'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                         's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      auto is = make_iterator_sentinel_source(tab);
      auto const v = read_all<std::vector<int>>(is);
      CHECK(std::equal(v.begin(), v.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("iterators")
    {
      char const tab[] = "abc\0defghijklmnopqrstuvwxyz";
      auto is = make_iterator_sentinel_source(std::begin(tab), std::end(tab));
      auto const s = read_all(is);
      CHECK(std::equal(s.begin(), s.end(), std::begin(tab), std::end(tab)));
    }

    SECTION("string literal")
    {
      SECTION("char")
      {
        auto is = make_iterator_sentinel_source("abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all(is);
        CHECK(s == "abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("wchar_t")
      {
        auto is = make_iterator_sentinel_source(L"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::wstring>(is);
        CHECK(s == L"abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("char16_t")
      {
        auto is = make_iterator_sentinel_source(u"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u16string>(is);
        CHECK(s == u"abcdefghijklmnopqrstuvwxyz");
      }

      SECTION("char32_t")
      {
        auto is = make_iterator_sentinel_source(U"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u32string>(is);
        CHECK(s == U"abcdefghijklmnopqrstuvwxyz");
      }

#ifdef MGS_HAS_CPP20
      SECTION("char8_t")
      {
        auto is = make_iterator_sentinel_source(u8"abcdefghijklmnopqrstuvwxyz");
        auto const s = read_all<std::u8string>(is);
        CHECK(s == u8"abcdefghijklmnopqrstuvwxyz");
      }
#endif
    }
  }

  SECTION("range")
  {
    static_assert(is_range<iterator_sentinel_source<char const*>>::value, "");
    static_assert(is_semiregular<iterator_sentinel_source<char const*>>::value,
                  "");

    auto source = make_iterator_sentinel_source("000");
    CHECK(std::count(source.begin(), source.end(), '0') == 3);
  }
}
