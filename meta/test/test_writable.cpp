#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/concepts/writable.hpp>

using namespace mgs::meta;
using namespace mgs;

namespace
{
struct It
{
  using value_type = std::string;
  using pointer = value_type*;
  using reference = value_type;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  reference operator*();
  It& operator++();
  It operator++(int);
};

// std::vector<bool>::iterator should be Writable, however the proxy it uses
// usually does not have const/rvalue overloads for operator=.
// Therefore write our own 'correct' proxy class.
struct It2
{
  struct Proxy
  {
    Proxy operator*() const;

    Proxy& operator=(std::string const&&) const;
    Proxy& operator=(std::string&&) const;
    Proxy& operator=(std::string const&) const;
    Proxy& operator=(std::string&) const;

    Proxy& operator=(Proxy const&) = default;
    Proxy& operator=(Proxy&&) = default;

    operator std::string() const;

    // Imagine having a std::string* as member
  };

  using value_type = std::string;
  using pointer = Proxy*;
  using reference = Proxy;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  reference operator*();
  It2& operator++();
  It2 operator++(int);
};
}

TEST_CASE("writable")
{
  static_assert(!is_writable<void, int>::value, "");
  static_assert(!is_writable<void*, int>::value, "");
  static_assert(!is_writable<struct incomplete*, int>::value, "");
  static_assert(!is_writable<struct incomplete, int>::value, "");

  static_assert(!is_writable<It, std::string>::value, "");
  static_assert(is_writable<It2, std::string>::value, "");
  static_assert(is_writable<char*, char>::value, "");
  static_assert(is_writable<char* const, char>::value, "");
  static_assert(!is_writable<char const*, char>::value, "");
  static_assert(is_writable<std::ostreambuf_iterator<char>, char>::value, "");
  static_assert(is_writable<std::back_insert_iterator<std::vector<int>>,
                            int const&>::value,
                "");
  static_assert(is_writable<std::unique_ptr<float>, int const&>::value, "");
}
