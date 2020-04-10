#ifndef BINVEC
#define BINVEC

#include <cstdint>
#include <vector>

#define ITYPE uint16_t
#define ISIZE 16

int sign(int val)
{
  return int(val >= 0);
}

class BinVec {

private:
  char int_container_size = ISIZE;
  std::vector<ITYPE> bits;
  int num_of_int_containers;
  int num_of_bits;

  // indexing proxy
  class IndexProxy {
  private:
    int idx;
    BinVec &ref;
  public:
    IndexProxy(BinVec & _ref, int _idx) : ref(_ref), idx(_idx) {}
    void operator= (int value);
    operator int() const;
  };

public:
  char size() const;
  int get_bit(int index) const;
  void set_bit(int value, int index);
  friend std::ostream & operator<< (std::ostream & os, const BinVec & vec);

  BinVec(int length);
  BinVec(int length, int with_bit);
  //BinVec(std::vector<int> other);

  int operator* (const BinVec &) const;
  BinVec operator+ (const BinVec &) const;
  friend BinVec operator* (int bit, const BinVec & bit_vector);
  friend int dot (int bit, const BinVec & bit_vector);
  IndexProxy operator[] (int idx);
  int operator[] (int idx) const;
};

#endif
