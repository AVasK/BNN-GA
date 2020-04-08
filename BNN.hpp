// I'm tired of multiple files, so no .cpp / .hpp separation this time
// Sorry for that
//
// - AVasK.


// Example of representation
// Layers: [2, 3, 1]
//
// Weights shapes per layer:
// [ 2 x [3],
//   3 x [1] ]
//
// Chromosome:
// [9 bits]

#include <iostream>
#include <cmath>
#include "binvec.hpp"
#include "myMap.hpp"


int sign(int val)
{
  return int(val >= 0);
}


float sigmoid(float val)
{
  return 1 / (1+exp(-val));
}

int threshold(float val)
{
  return int(val > 0.5);
}


template <typename T>
BinVec dot (std::vector<T> activations, std::vector<BinVec> bin_vec)
{
  BinVec result = activations[0] * bin_vec[0];
  for (int i = 1; i < activations.size(); i++)
  {
    std::cout << "res: " << result;
    std::cout << " + " << activations[i] * bin_vec[i] << "\n";
    result = result + activations[i] * bin_vec[i];
    std::cout << " = " << result << "\n\n";
  }
  return result;
}

// BAD AND SILLY, FOR TESTING PURPOSES ONLY
std::vector<int> to_vec(BinVec v)
{
  std::vector<int> res (v.size());
  for (int i = 0; i < v.size(); i++)
  {
    res[i] = v[i];
  }
  return res;
}


class BNN {

private:

  std::vector<int> layers;
  std::vector< std::vector<BinVec> > weights_per_layer;

public:

  BNN(std::vector<int> _layers)
  {
    layers = _layers;
    for (int i = 0; i < layers.size()-1; i++)
    {
      std::vector<BinVec> layer_weights (layers[i], BinVec(0)); // BinVec(0) is a placeholder
      for (auto & elem : layer_weights)
      {
        elem = BinVec(layers[i+1]); // the real initialization
      }
      weights_per_layer.push_back(layer_weights);
    }
  }


  friend std::ostream & operator<< (std::ostream &os, const BNN & net)
  {
    for (int layer_idx = 0; layer_idx < net.weights_per_layer.size(); layer_idx++)
    {
      auto layer_weights = net.weights_per_layer[layer_idx];

      os << "Weights between layers " << layer_idx
         << " and " << layer_idx+1 << "\n";

      for (auto elem : layer_weights)
      {
        os << elem << "\n";
      }
    }
    return os;
  }


  std::vector<float> forward(std::vector<int> input)
  {

  }


};
