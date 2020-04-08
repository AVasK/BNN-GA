#include <iostream>

#include "binvec.cpp"
#include "BNN.hpp"

int main()
{
  /*
  auto size = 5;
  auto bv = BinVec(size);
  auto bw = BinVec(size);

  std::cout << bv << "\n";
  //bv[2] = 1;
  //bv[1] = 0;
  //bv[0] = 1;
  std::cout << bv << "\n";
  std::cout << bw << "\n";
  std::cout << bv*bw << "\n";
  std::cout << bv+bw << "\n";

  std::cout << 1 * bv;
  std::cout << 0 * bv;
  std::cout << "\n";
  */

  auto net = BNN(std::vector<int>({2,3,1}));
  std::cout << net;
  //net.forward(std::vector<int>({1,-1}));
}
