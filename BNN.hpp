// I'm tired of multiple files, so no .cpp / .hpp separation this time
// Sorry for that
//
// - AVasK.


#include <iostream>
#include <cmath>
#include "binvec.cpp"
#include "binmat.hpp"
#include "myMap.hpp"


float sigmoid(float val)
{
  return 1.0 / (1.0+exp(-val));
}

int threshold(float val)
{
  return int(val > 0.5);
}



class BNN {

private:

  std::vector<int> layers;
  std::vector<BinMat> weights;

public:

  BNN(std::vector<int> _layers)
  {
    layers = _layers;
    for (int i = 0; i < layers.size()-1; i++)
    {
      auto input_size = layers[i];
      auto output_size = layers[i+1];
      auto weights_per_layer = BinMat(input_size, output_size);
      weights.push_back(weights_per_layer);
    }
  }

  // Naive implementation, may be slow
  // BinVec weightsToBinVec()

  unsigned long size() const
  {
    unsigned long sum = 0;
    for (const auto & weight : weights)
    {
      sum += weight.size();
    }
    return sum;
  }

  friend std::ostream & operator<< (std::ostream &os, const BNN & net)
  {
    for (const auto & layer_size : net.layers) os << layer_size << ", ";
    os << "\nweights:\n";
    for (const auto & weight_mat : net.weights) os << weight_mat << "\n";
    return os;
  }


  float forward(std::vector<int> input) const
  {
    auto activations = BinFromVec(input);
    //std::cout << "activations: "<< activations << "\n";
    for (int i = 0; i < weights.size() - 1; i++)
    {
      activations = activations * weights[i];
      //std::cout << "activations: "<< activations << "\n";
    }
    int i = weights.size() - 1;
    std::cout << realValuedDot(activations, weights[i])[0] << "\n";
    return sigmoid(realValuedDot(activations, weights[i])[0]);
  }


};
