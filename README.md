# BNN + GA

## Applying Genetic Algorithm to Binarized Neural Network training
As a result, we don't need to keep the Real-Valued weights at all,
compared to all other methods.

*PROTOTYPED* in Python

Core binary operations & classes written in C++:
- Binary Vector (BinVec) stores binary activations space-efficiently, uses popcount & xnor ops for dot product
- Binary Matrix (BinMat) stores columns of binary vectors for efficient (activations * weights) multiplication
- Binary Network(BNN) works using BinVec for activations and BinMat for weight matrices in all layers except for the last, which uses realValuedDot(binVec, binMat) -> std::vector<float> since we may need to use some non-binary activation function.
