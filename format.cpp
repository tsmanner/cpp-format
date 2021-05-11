#include <iomanip>
#include <iostream>

#include "Formatter.h"


constexpr auto MyFmt = "My number is " + Fmt<int, 5>() + "! My char is '" + Fmt<char>() + "'!\n";
constexpr auto EaFmt = "Ea=" + Fmt<uint32_t, 8, '0', std::hex>() + ":" + Fmt<uint32_t, 8, '0', std::hex, std::uppercase>();


int main() {
  using std::cout;

  MyFmt(cout, 15, 'x');
  cout << MyFmt(1234567890, 'y');

  uint64_t ea =0xDEADBEEFDEADBEEF;
  uint64_t aa =0xEFDEADBEEF;

  EaFmt(cout, uint32_t(ea >> 32), uint32_t(ea));
  cout << '\n';

  ("Aa=" + Fmt<uint32_t, 5, '0', std::hex>() + ":" + Fmt<uint32_t, 8, '0', std::hex>())(cout, uint32_t(aa >> 32), uint32_t(aa));
  cout << '\n';

  return 0;
}
