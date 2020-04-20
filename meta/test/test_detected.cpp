#include <functional>
#include <map>
#include <vector>

#include <catch2/catch.hpp>

#include <mgs/meta/detected.hpp>

#include <mgs/meta/detected/operators/addition.hpp>
#include <mgs/meta/detected/operators/addition_assignment.hpp>
#include <mgs/meta/detected/operators/array_subscript.hpp>
#include <mgs/meta/detected/operators/dereference.hpp>
#include <mgs/meta/detected/operators/function_call.hpp>
#include <mgs/meta/detected/operators/greater_or_equal_than.hpp>
#include <mgs/meta/detected/operators/greater_than.hpp>
#include <mgs/meta/detected/operators/less_or_equal_than.hpp>
#include <mgs/meta/detected/operators/less_than.hpp>
#include <mgs/meta/detected/operators/post_decrement.hpp>
#include <mgs/meta/detected/operators/post_increment.hpp>
#include <mgs/meta/detected/operators/pre_decrement.hpp>
#include <mgs/meta/detected/operators/pre_increment.hpp>
#include <mgs/meta/detected/operators/substraction.hpp>
#include <mgs/meta/detected/operators/substraction_assignment.hpp>

#include <mgs/meta/detected/types/difference_type.hpp>
#include <mgs/meta/detected/types/iterator.hpp>
#include <mgs/meta/detected/types/iterator_category.hpp>
#include <mgs/meta/detected/types/key_type.hpp>
#include <mgs/meta/detected/types/pointer.hpp>
#include <mgs/meta/detected/types/reference.hpp>
#include <mgs/meta/detected/types/value_type.hpp>

using namespace mgs::meta;
using namespace mgs::meta::detected;

namespace
{
struct incomplete;

struct non_const_array_subscript
{
  void operator[](int);
};

struct non_const_dereference
{
  void operator*();
};

struct non_const_function_call
{
  void operator()();
};

struct invalid_totally_ordered
{
  invalid_totally_ordered(int);

  void operator<(invalid_totally_ordered);
  void operator>(invalid_totally_ordered);
  void operator>=(invalid_totally_ordered);
  void operator<=(invalid_totally_ordered);
};

struct valid_totally_ordered
{
  valid_totally_ordered(int);
};

void operator<(valid_totally_ordered, valid_totally_ordered);
void operator>(valid_totally_ordered, valid_totally_ordered);
void operator<=(valid_totally_ordered, valid_totally_ordered);
void operator>=(valid_totally_ordered, valid_totally_ordered);
}

TEST_CASE("detected")
{
  SECTION("operators")
  {
    SECTION("addition")
    {
      static_assert(is_detected<operators::addition, int, int>::value, "");
      static_assert(
          is_detected<operators::addition, std::string, std::string>::value,
          "");

      static_assert(!is_detected<operators::addition, void* const&, void*>::value, "");
      static_assert(!is_detected<operators::addition, void, void>::value, "");
      static_assert(
          !is_detected<operators::addition, incomplete, incomplete>::value, "");
    }

    SECTION("addition assignment")
    {
      static_assert(is_detected<operators::addition_assignment, int&, int>::value, "");
      static_assert(is_detected<operators::addition_assignment,
                                std::string&,
                                std::string>::value,
                    "");
      static_assert(is_detected<operators::addition_assignment,
                                std::string,
                                std::string>::value,
                    "");

      static_assert(!is_detected<operators::addition_assignment,
                                 void*,
                                 void* const&>::value,
                    "");
      static_assert(
          !is_detected<operators::addition_assignment, void, void>::value, "");
      static_assert(!is_detected<operators::addition_assignment,
                                 incomplete,
                                 incomplete>::value,
                    "");
    }

    SECTION("array subscript")
    {
      static_assert(is_detected<operators::array_subscript,
                                std::string&,
                                std::size_t>::value,
                    "");
      static_assert(is_detected<operators::array_subscript,
                                std::string const&,
                                std::size_t>::value,
                    "");
      static_assert(is_detected<operators::array_subscript,
                                char const*,
                                std::size_t>::value,
                    "");
      static_assert(is_detected<operators::array_subscript,
                                non_const_array_subscript,
                                std::size_t>::value,
                    "");

      static_assert(!is_detected<operators::array_subscript,
                                 non_const_array_subscript const&,
                                 std::size_t>::value,
                    "");
      static_assert(!is_detected<operators::array_subscript,
                                 void*,
                                 std::size_t>::value,
                    "");
      static_assert(!is_detected<operators::array_subscript,
                                 void,
                                 std::size_t>::value,
                    "");
      static_assert(!is_detected<operators::array_subscript,
                                 incomplete,
                                 std::size_t>::value,
                    "");
    }

    SECTION("dereference")
    {
      static_assert(is_detected<operators::dereference, int*>::value, "");
      static_assert(is_detected<operators::dereference,
                                std::map<int, int>::iterator>::value,
                    "");
      static_assert(
          is_detected<operators::dereference, non_const_dereference>::value,
          "");

      static_assert(
          std::is_same<
              detected_t<operators::dereference, non_const_dereference const>,
              nonesuch>::value,
          "");
      static_assert(!is_detected<operators::dereference,
                                 non_const_dereference const>::value,
                    "");
      static_assert(!is_detected<operators::dereference, void>::value, "");
      static_assert(!is_detected<operators::dereference, void*>::value, "");
    }

    SECTION("function call")
    {
      static_assert(is_detected<operators::function_call,
                                std::function<void(int)>,
                                int>::value,
                    "");
      static_assert(
          is_detected<operators::function_call, std::function<void()> const>::value,
          "");
      static_assert(
          is_detected<operators::function_call, std::plus<int>, int, int>::
              value,
          "");
      static_assert(
          is_detected<operators::function_call, non_const_function_call>::value,
          "");

      static_assert(
          !is_detected<operators::function_call, non_const_function_call const>::value,
          "");
      static_assert(!is_detected<operators::function_call, void>::value, "");
      static_assert(!is_detected<operators::function_call, void*>::value, "");
      static_assert(!is_detected<operators::function_call, incomplete>::value, "");
    }

    SECTION("greater or equal than")
    {
      static_assert(
          is_detected<operators::greater_or_equal_than, int, int>::value, "");
      static_assert(is_detected<operators::greater_or_equal_than,
                                invalid_totally_ordered,
                                invalid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::greater_or_equal_than,
                                invalid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::greater_or_equal_than,
                                valid_totally_ordered,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::greater_or_equal_than,
                                valid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::greater_or_equal_than,
                                int,
                                valid_totally_ordered>::value,
                    "");
      static_assert(
          is_detected<operators::greater_or_equal_than, void*, void*>::value,
          "");

      static_assert(!is_detected<operators::greater_or_equal_than,
                                 int,
                                 invalid_totally_ordered>::value,
                    "");
      static_assert(
          !is_detected<operators::greater_or_equal_than, void, void>::value,
          "");
      static_assert(!is_detected<operators::greater_or_equal_than,
                                 incomplete,
                                 incomplete>::value,
                    "");
    }

    SECTION("greater than")
    {
      static_assert(is_detected<operators::greater_than, int, int>::value, "");
      static_assert(is_detected<operators::greater_than,
                                invalid_totally_ordered,
                                invalid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::greater_than,
                                invalid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::greater_than,
                                valid_totally_ordered,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::greater_than,
                                valid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::greater_than,
                                int,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::greater_than, void*, void*>::value,
                    "");

      static_assert(!is_detected<operators::greater_than,
                                 int,
                                 invalid_totally_ordered>::value,
                    "");
      static_assert(!is_detected<operators::greater_than, void, void>::value,
                    "");
      static_assert(
          !is_detected<operators::greater_than, incomplete, incomplete>::value,
          "");
    }

    SECTION("less than")
    {
      static_assert(is_detected<operators::less_than, int, int>::value, "");
      static_assert(is_detected<operators::less_than,
                                invalid_totally_ordered,
                                invalid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_than,
                                invalid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::less_than,
                                valid_totally_ordered,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_than,
                                valid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::less_than,
                                int,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_than, void*, void*>::value, "");

      static_assert(!is_detected<operators::less_than,
                                 int,
                                 invalid_totally_ordered>::value,
                    "");
      static_assert(!is_detected<operators::less_than, void, void>::value, "");
      static_assert(
          !is_detected<operators::less_than, incomplete, incomplete>::value,
          "");
    }

    SECTION("less or equal than")
    {
      static_assert(is_detected<operators::less_or_equal_than, int, int>::value, "");
      static_assert(is_detected<operators::less_or_equal_than,
                                invalid_totally_ordered,
                                invalid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_or_equal_than,
                                invalid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::less_or_equal_than,
                                valid_totally_ordered,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_or_equal_than,
                                valid_totally_ordered,
                                int>::value,
                    "");
      static_assert(is_detected<operators::less_or_equal_than,
                                int,
                                valid_totally_ordered>::value,
                    "");
      static_assert(is_detected<operators::less_or_equal_than, void*, void*>::value, "");

      static_assert(!is_detected<operators::less_or_equal_than,
                                 int,
                                 invalid_totally_ordered>::value,
                    "");
      static_assert(!is_detected<operators::less_or_equal_than, void, void>::value, "");
      static_assert(!is_detected<operators::less_or_equal_than,
                                 incomplete,
                                 incomplete>::value,
                    "");
    }

    SECTION("post increment")
    {
      static_assert(is_detected<operators::post_increment, int&>::value, "");
      static_assert(is_detected<operators::post_increment,
                                std::map<int, int>::iterator&>::value,
                    "");
      static_assert(is_detected<operators::post_increment,
                                std::map<int, int>::iterator>::value,
                    "");

      static_assert(!is_detected<operators::post_increment, int>::value, "");
      static_assert(!is_detected<operators::post_increment, int const&>::value, "");
      static_assert(!is_detected<operators::post_increment, void>::value, "");
      static_assert(!is_detected<operators::post_increment, void*>::value, "");
      static_assert(!is_detected<operators::post_increment, incomplete*>::value, "");
      static_assert(!is_detected<operators::post_increment, incomplete>::value, "");
    }

    SECTION("pre increment")
    {
      static_assert(is_detected<operators::pre_increment, int&>::value, "");
      static_assert(is_detected<operators::pre_increment,
                                std::map<int, int>::iterator&>::value,
                    "");
      static_assert(is_detected<operators::pre_increment,
                                std::map<int, int>::iterator>::value,
                    "");

      static_assert(!is_detected<operators::pre_increment, int>::value, "");
      static_assert(!is_detected<operators::pre_increment, int const&>::value, "");
      static_assert(!is_detected<operators::pre_increment, void>::value, "");
      static_assert(!is_detected<operators::pre_increment, void*>::value, "");
      static_assert(!is_detected<operators::pre_increment, incomplete*>::value, "");
      static_assert(!is_detected<operators::pre_increment, incomplete>::value, "");
    }

    SECTION("post decrement")
    {
      static_assert(is_detected<operators::post_decrement, int&>::value, "");
      static_assert(is_detected<operators::post_decrement,
                                std::map<int, int>::iterator&>::value,
                    "");
      static_assert(is_detected<operators::post_decrement,
                                std::map<int, int>::iterator>::value,
                    "");

      static_assert(!is_detected<operators::post_decrement, int>::value, "");
      static_assert(!is_detected<operators::post_decrement, int const&>::value,
                    "");
      static_assert(!is_detected<operators::post_decrement, void>::value, "");
      static_assert(!is_detected<operators::post_decrement, void*>::value, "");
      static_assert(!is_detected<operators::post_decrement, incomplete*>::value,
                    "");
      static_assert(!is_detected<operators::post_decrement, incomplete>::value,
                    "");
    }

    SECTION("pre decrement")
    {
      static_assert(is_detected<operators::pre_decrement, int&>::value, "");
      static_assert(is_detected<operators::pre_decrement,
                                std::map<int, int>::iterator&>::value,
                    "");
      static_assert(is_detected<operators::pre_decrement,
                                std::map<int, int>::iterator>::value,
                    "");

      static_assert(!is_detected<operators::pre_decrement, int>::value, "");
      static_assert(!is_detected<operators::pre_decrement, int const&>::value,
                    "");
      static_assert(!is_detected<operators::pre_decrement, void>::value, "");
      static_assert(!is_detected<operators::pre_decrement, void*>::value, "");
      static_assert(!is_detected<operators::pre_decrement, incomplete*>::value,
                    "");
      static_assert(!is_detected<operators::pre_decrement, incomplete>::value,
                    "");
    }

    SECTION("substraction")
    {
      static_assert(is_detected<operators::substraction, int, int>::value, "");

      static_assert(!is_detected<operators::substraction, void* const&, void*>::value, "");
      static_assert(!is_detected<operators::substraction, void, void>::value, "");
      static_assert(
          !is_detected<operators::substraction, incomplete, incomplete>::value, "");
    }

    SECTION("substraction assignment")
    {
      static_assert(is_detected<operators::substraction_assignment, int&, int>::value, "");

      static_assert(!is_detected<operators::substraction_assignment, int, int>::value, "");
      static_assert(!is_detected<operators::substraction_assignment, int const&, int>::value, "");
      static_assert(!is_detected<operators::substraction_assignment,
                                 void*,
                                 void* const&>::value,
                    "");
      static_assert(
          !is_detected<operators::substraction_assignment, void, void>::value, "");
      static_assert(!is_detected<operators::substraction_assignment,
                                 incomplete,
                                 incomplete>::value,
                    "");
    }
  }

  SECTION("types")
  {
    SECTION("difference_type")
    {
      static_assert(is_detected<types::difference_type,
                                std::map<int, int>::iterator>::value,
                    "");
      static_assert(is_detected<types::difference_type,
                                std::iterator_traits<char*>>::value,
                    "");

      static_assert(!is_detected<types::difference_type, char*>::value, "");
      static_assert(!is_detected<types::difference_type, void*>::value, "");
      static_assert(!is_detected<types::difference_type, void>::value, "");
      static_assert(!is_detected<types::difference_type, incomplete>::value,
                    "");
    }

    SECTION("iterator")
    {
      static_assert(is_detected<types::iterator, std::map<int, int>>::value,
                    "");

      static_assert(!is_detected<types::iterator, void*>::value, "");
      static_assert(!is_detected<types::iterator, void>::value, "");
      static_assert(!is_detected<types::iterator, incomplete>::value,
                    "");
    }

    SECTION("iterator_category")
    {
      static_assert(is_detected<types::iterator_category,
                                std::map<int, int>::iterator>::value,
                    "");
      static_assert(is_detected<types::iterator_category,
                                std::iterator_traits<char*>>::value,
                    "");

      static_assert(!is_detected<types::iterator_category, char*>::value, "");
      static_assert(!is_detected<types::iterator_category, void*>::value, "");
      static_assert(!is_detected<types::iterator_category, void>::value, "");
      static_assert(!is_detected<types::iterator_category, incomplete>::value,
                    "");
    }

    SECTION("key_type")
    {
      static_assert(is_detected<types::key_type, std::map<int, int>>::value,
                    "");

      static_assert(!is_detected<types::key_type, char*>::value, "");
      static_assert(!is_detected<types::key_type, void*>::value, "");
      static_assert(!is_detected<types::key_type, void>::value, "");
      static_assert(!is_detected<types::key_type, incomplete>::value,
                    "");
    }

    SECTION("reference")
    {
      static_assert(
          is_detected<types::reference, std::map<int, int>::iterator>::value,
          "");
      static_assert(
          is_detected<types::reference, std::iterator_traits<char*>>::value,
          "");

      static_assert(!is_detected<types::reference, char*>::value, "");
      static_assert(!is_detected<types::reference, void*>::value, "");
      static_assert(!is_detected<types::reference, void>::value, "");
      static_assert(!is_detected<types::reference, incomplete>::value, "");
    }

    SECTION("value_type")
    {
      static_assert(
          is_detected<types::value_type, std::map<int, int>::iterator>::value,
          "");
      static_assert(
          is_detected<types::value_type, std::iterator_traits<char*>>::value,
          "");

      static_assert(!is_detected<types::value_type, char*>::value, "");
      static_assert(!is_detected<types::value_type, void*>::value, "");
      static_assert(!is_detected<types::value_type, void>::value, "");
      static_assert(!is_detected<types::value_type, incomplete>::value, "");
    }
  }
}
