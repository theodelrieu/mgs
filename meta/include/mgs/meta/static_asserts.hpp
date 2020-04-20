#pragma once

#include <tuple>

namespace mgs
{
namespace detail
{
template <typename T, typename SubRequirements, typename = void>
struct collect_requirements;

template <typename T>
struct collect_requirements<T, std::tuple<>>
{
  using type = std::tuple<T>;
};

template <typename T, typename... SubRequirements>
struct collect_requirements<T,
                            std::tuple<SubRequirements...>,
                            std::enable_if_t<sizeof...(SubRequirements) != 0>>
{
  using type = decltype(
      std::tuple_cat(std::tuple<T>{},
                     typename collect_requirements<
                         SubRequirements,
                         typename SubRequirements::requirements>::type{}...));
};

template <typename... U>
struct filter_requirements_impl;

template <>
struct filter_requirements_impl<>
{
  using type = std::tuple<>;
};

template <typename Requirement, typename... Tail>
struct filter_requirements_impl<Requirement, Tail...>
{
  using type = decltype(
      std::tuple_cat(std::conditional_t<!Requirement::value,
                                        std::tuple<Requirement>,
                                        std::tuple<>>{},
                     typename filter_requirements_impl<Tail...>::type{}));
};

template <typename T>
struct filter_requirements;

template <typename... Requirements>
struct filter_requirements<std::tuple<Requirements...>>
{
  using type = typename filter_requirements_impl<Requirements...>::type;
};

template <typename ...Requirements>
struct collect_failed_requirements;

template <typename End>
struct collect_failed_requirements<End>
{
  using type = typename filter_requirements<
      typename collect_requirements<End,
                                    typename End::requirements>::type>::type;
};

template <typename Head, typename... Tail>
struct collect_failed_requirements<Head, Tail...>
{
  using AllRequirements =
      typename collect_requirements<Head, typename Head::requirements>::type;

  using type = decltype(std::tuple_cat(
      AllRequirements{},
      std::declval<typename filter_requirements<
          typename collect_failed_requirements<Tail...>::type>::type>()));
};

//  typename filter_requirements<AllRequirements>::type;
template <typename Requirement>
constexpr int print_static_asserts()
{
  return Requirement::trigger_static_asserts();
}

template <typename Requirement, typename... Requirements>
constexpr auto print_static_asserts()
    -> std::enable_if_t<sizeof...(Requirements) != 0, int>
{
  constexpr auto i = Requirement::trigger_static_asserts();
  return i + print_static_asserts<Requirements...>();
}

template <typename Requirements>
struct trigger_static_asserts;

template <typename ...Requirements>
struct trigger_static_asserts<std::tuple<Requirements...>>
{
  static constexpr auto trigger()
  {
    return print_static_asserts<Requirements...>();
  }
};

template <>
struct trigger_static_asserts<std::tuple<>>
{
  static constexpr auto trigger()
  {
    return 0;
  }
};
}

namespace meta
{
template <typename ...Requirements>
constexpr int trigger_static_asserts()
{
  return detail::trigger_static_asserts<
      typename detail::collect_failed_requirements<Requirements...>::type>::
      trigger();
}
}
}
