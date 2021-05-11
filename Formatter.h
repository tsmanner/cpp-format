#pragma once

#include <iostream>
#include <sstream>
#include <tuple>
#include <type_traits>

#include "ConstString.h"
#include "Fmt.h"


//
// Args helper struct used to construct a tuple-type of all Fmt
// argument types.  The result is used by the Formatter to build
// the argument list for the call operator.
//

// Args declaration
template <typename... T>
struct Args;

// Args specialization for no types
template <>
struct Args<> {
  using types = std::tuple<>;
};

// Args specialization for a ConstString
//   Contributes no arguments
template <typename T>
struct Args<T, typename std::enable_if_t<(std::is_same<T, ConstString>::value)>> {
  using types = std::tuple<>;
};

// Args specialization for a Fmt type
//   Contributes the type argument of the Fmt
template <typename T, int Width, char Fill, FmtFunctionType ...IosFlags>
struct Args<Fmt<T, Width, Fill, IosFlags...>, void> {
  using types = std::tuple<T>;
};

// Args specialization for multiple types
//   Uses tuple_cat to combine the arguments from the first type
//   with the arguments from a recursive call Args with the
//   remaining types.
template <typename T, typename... Ts>
struct Args<T, Ts...> {
  using types = decltype(std::tuple_cat(typename Args<T, void>::types(), typename Args<Ts...>::types()));
};


//
// Formatter struct
//   This class is the
//

template <typename... ElementTypes>
struct Formatter {
  constexpr Formatter(std::tuple<ElementTypes...> const &inElements): mElements(inElements) {}

  std::tuple<ElementTypes...> mElements;
  using ArgsTuple = typename Args<ElementTypes...>::types;

  template <std::size_t ElementsI, std::size_t ArgsI>
  typename std::enable_if_t<(ElementsI == sizeof...(ElementTypes))>
  stream(std::ostream &, ArgsTuple const &) const {}

  template <std::size_t ElementsI, std::size_t ArgsI, typename T, typename... Ts>
  typename std::enable_if_t<(
    ElementsI != sizeof...(ElementTypes) and
    std::is_same<ConstString, T>::value
  )>
  stream(std::ostream &os, ArgsTuple const &inArgs) const {
    os << std::get<ElementsI>(mElements);
    stream<ElementsI+1, ArgsI, Ts...>(os, inArgs);
  }

  template <std::size_t ElementsI, std::size_t ArgsI, typename T, typename... Ts>
  typename std::enable_if_t<(
    ElementsI != sizeof...(ElementTypes) and
    !std::is_same<ConstString, T>::value
  )>
  stream(std::ostream &os, ArgsTuple const &inArgs) const {
    os << std::get<ElementsI>(mElements) << std::get<ArgsI>(inArgs);
    stream<ElementsI+1, ArgsI+1, Ts...>(os, inArgs);
  }


  template <typename... ArgTs>
  typename std::enable_if_t<(std::is_same<std::tuple<ArgTs...>, ArgsTuple>::value)>
  operator()(std::ostream &os, ArgTs const &... inArgs) const {
    stream<0, 0, ElementTypes...>(os, ArgsTuple(inArgs...));
  }

  template <typename... ArgTs>
  typename std::enable_if_t<(std::is_same<std::tuple<ArgTs...>, ArgsTuple>::value), std::string>
  operator()(ArgTs const &... inArgs) const {
    std::stringstream ss;
    operator()(ss, inArgs...);
    return ss.str();
  }

};


template <typename T, int Width, char Fill, FmtFunctionType ...IosFlags>
constexpr auto operator+(ConstString const &lhs, Fmt<T, Width, Fill, IosFlags...> const &rhs) {
  return Formatter<ConstString, Fmt<T, Width, Fill, IosFlags...>>(std::make_tuple(lhs, rhs));
}

template <typename T, int Width, char Fill, FmtFunctionType ...IosFlags>
constexpr auto operator+(Fmt<T, Width, Fill, IosFlags...> const &lhs, ConstString const &rhs) {
  return Formatter<Fmt<T, Width, Fill, IosFlags...>, ConstString>(std::make_tuple(lhs, rhs));
}

template <typename... Ts>
constexpr auto operator+(Formatter<Ts...> const &lhs, ConstString const &rhs) {
  return Formatter<Ts..., ConstString>(std::tuple_cat(lhs.mElements, std::make_tuple(rhs)));
}

template <typename... Ts>
constexpr auto operator+(ConstString const &lhs, Formatter<Ts...> const &rhs) {
  return Formatter<ConstString, Ts...>(std::tuple_cat(std::make_tuple(lhs), rhs.mElements));
}

template <typename T, int Width, char Fill, FmtFunctionType ...IosFlags, typename... Ts>
constexpr auto operator+(Formatter<Ts...> const &lhs, Fmt<T, Width, Fill, IosFlags...> const &rhs) {
  return Formatter<Ts..., Fmt<T, Width, Fill, IosFlags...>>(std::tuple_cat(lhs.mElements, std::make_tuple(rhs)));
}

template <typename T, int Width, char Fill, FmtFunctionType ...IosFlags, typename... Ts>
constexpr auto operator+(Fmt<T, Width, Fill, IosFlags...> const &lhs, Formatter<Ts...> const &rhs) {
  return Formatter<Fmt<T, Width, Fill, IosFlags...>, Ts...>(std::tuple_cat(std::make_tuple(lhs), rhs.mElements));
}
