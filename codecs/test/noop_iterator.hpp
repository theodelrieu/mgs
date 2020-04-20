#include <iterator>

namespace test_helpers
{
struct noop_iterator
{
  using value_type = void;
  using pointer = void;
  using reference = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::output_iterator_tag;

  noop_iterator& operator++()
  {
    return *this;
  }

  noop_iterator& operator++(int)
  {
    return *this;
  }

  noop_iterator& operator*()
  {
    return *this;
  }

  template <typename T>
  noop_iterator& operator=(T&&)
  {
    return *this;
  }
};
}
