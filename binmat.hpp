#ifndef BINMAT
#define BINMAT

#include <iostream>
#include <vector>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include "binvec.hpp"

// stores BinVec by columns
// activations x BinMat -> pre-activations of the next layer
// a[1 x n] x BinMat[n x k] -> a[1 x k],
// activations stored as BinVec(!!!), dimms are non-standard [1 x n],
// SO THE ACTIVATIONS ARE STORED AS A ROW-VECTOR (!!!)
// THIS IS THE WAY to harness all the benefits of popcount & xnor optimizations.

class BinMat
{
private:
  std::vector<BinVec> columns;
  unsigned long _num_columns;

public:
  // initialize with vec of columns
  BinMat(std::vector<BinVec> cols) : columns(cols), _num_columns(cols.size()) {}

  // random init
  BinMat(unsigned long n_rows, unsigned long n_cols) : _num_columns(n_cols)
  {
    for (unsigned long col = 0; col < n_cols; col++)
    {
      columns.push_back(BinVec(n_rows));
    }
  }

  // Copy from existing
  BinMat(const BinMat & copy_from)
  {
    columns = copy_from.columns;
    _num_columns = copy_from._num_columns;
  }


  unsigned long n_rows() const { return columns[0].size(); }


  unsigned long long size() const {
    return _num_columns * n_rows();
  }


  // BinVec * BinMat -> BinVec
  friend BinVec operator* (BinVec activations, const BinMat & weights)
  {
    // dimm-check
    auto _binvec_size = weights.n_rows();
    if (_binvec_size != activations.size())
    {
      throw std::length_error("Dimmensions are off");
    }

    BinVec new_activations (weights._num_columns, 0); // zero binvec
    unsigned long i = 0;
    for (auto& weight_vector : weights.columns)
    {
      new_activations[i++] = sign(activations * weight_vector);
    }
    return new_activations;
  }


  // BinVec, BinMat -> std::vector<int>
  friend std::vector<int>
  realValuedDot(BinVec activations, const BinMat & weights)
  {
    auto _binvec_size = weights.n_rows();

    // dimm-check
    if (_binvec_size != activations.size())
    {
      throw std::length_error("Dimmensions are off");
    }

    std::vector<int> new_activations (weights._num_columns, 0);
    unsigned long i = 0;
    for (auto& weight_vector : weights.columns)
    {
      new_activations[i++] = activations * weight_vector;
    }
    return new_activations;
  }


  // Efficiency & cache-optimizations are out of concern here
  // after all, what kind of weirdo will print out a huge matrix?
  // so, don't do that.
  friend std::ostream & operator<< (std::ostream & os, const BinMat & mat)
  {
    for (unsigned long row = 0; row < mat.n_rows(); row++)
    {
      for (auto& col : mat.columns)
      {
        os << " " << col[row];
      }
      os << "\n";
    }
    return os;
  }

};

#endif
