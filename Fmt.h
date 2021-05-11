#pragma once

#include <iostream>
#include <type_traits>

using FmtFunctionType = std::ios_base&(*)(std::ios_base&);
template <typename T, int Width = 0, char Fill = 0, FmtFunctionType... IosFlags>
struct Fmt {};


template <FmtFunctionType... IosFlags>
inline typename std::enable_if_t<(sizeof...(IosFlags) == 0)>
streamFmtFunctions(std::ostream &) {}

template <FmtFunctionType IosFlag, FmtFunctionType... IosFlags>
inline typename std::enable_if_t<(sizeof...(IosFlags) == 0)>
streamFmtFunctions(std::ostream &os) {
  os << IosFlag;
}

template <FmtFunctionType IosFlag, FmtFunctionType... IosFlags>
inline typename std::enable_if_t<(sizeof...(IosFlags) != 0)>
streamFmtFunctions(std::ostream &os) {
  os << IosFlag;
  streamFmtFunctions<IosFlags...>(os);
}


template <typename T, int Width, char Fill, FmtFunctionType... IosFlags>
typename std::enable_if_t<(Width == 0 and Fill == 0), std::ostream &>
operator<<(std::ostream &os, Fmt<T, Width, Fill, IosFlags...> const &) {
  streamFmtFunctions<IosFlags...>(os);
  return os;
};

template <typename T, int Width, char Fill, FmtFunctionType... IosFlags>
typename std::enable_if_t<(Width == 0 and Fill != 0), std::ostream &>
operator<<(std::ostream &os, Fmt<T, Width, Fill, IosFlags...> const &) {
  streamFmtFunctions<IosFlags...>(os);
  return os << std::setfill(Fill);
};

template <typename T, int Width, char Fill, FmtFunctionType... IosFlags>
typename std::enable_if_t<(Width != 0 and Fill == 0), std::ostream &>
operator<<(std::ostream &os, Fmt<T, Width, Fill, IosFlags...> const &) {
  streamFmtFunctions<IosFlags...>(os);
  return os << std::setw(Width);
};

template <typename T, int Width, char Fill, FmtFunctionType... IosFlags>
typename std::enable_if_t<(Width != 0 and Fill != 0), std::ostream &>
operator<<(std::ostream &os, Fmt<T, Width, Fill, IosFlags...> const &) {
  streamFmtFunctions<IosFlags...>(os);
  return os << std::setw(Width) << std::setfill(Fill);
};
