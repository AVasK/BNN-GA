#include <iostream>

#include "binvec.hpp"
#include "binmat.hpp"
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

  std::cout << "mask: " << to_binary(mask_last(4), 32) << "\n";

  auto bv = BinVec(3);
  std::cout << bv << "\n*\n";

  auto mat = BinMat(3, 3);
  std::cout << mat << "\n";
  std::cout << "total #weights = " << mat.size() << "\n";
  auto res = bv*mat;
  std::cout << "done\n";
  std::cout << res << " " << res.size() << "\n";
  //std::cout << "\n=\n" << bv*mat << "\n";
  //std::cout << realValuedDot(bv, mat) << "\n";

  for (int idx=0; idx<3; idx++)
  {
    std::cout << (bv*mat)[idx] << " ";
  }


  auto net = BNN(std::vector<int>({2,3,1}));
  std::cout << net;
  //net.forward(std::vector<int>({1,-1}));
}
