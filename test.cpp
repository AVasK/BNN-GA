#include <iostream>

#include "binvec.cpp"

int main()
{
  auto size = 5;
  auto bv = BinVec(size);
  auto bw = BinVec(size, 0);

  std::cout << bv << "\n";
  bv[2] = 1;
  bv[1] = 0;
  bv[0] = 1;
  std::cout << bv << "\n";
  std::cout << bw << "\n";
  std::cout << bv*bw << "\n";
  std::cout << 1*bv << "\n";
}
