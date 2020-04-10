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
  unsigned int num_of_int_containers;
  unsigned long num_of_bits;

  // indexing proxy
  class IndexProxy {
  private:
    unsigned long idx;
    BinVec &ref;
  public:
    IndexProxy(BinVec & _ref, unsigned long _idx) : ref(_ref), idx(_idx) {}
    void operator= (int value);
    operator int() const;
  };

public:
  unsigned long size() const;
  int get_bit(unsigned long index) const;
  void set_bit(int value, unsigned long index);
  friend std::ostream & operator<< (std::ostream & os, const BinVec & vec);

  BinVec(unsigned long length);
  BinVec(unsigned long length, int with_bit);
  friend BinVec BinFromVec(std::vector<int> bits);
  //BinVec(std::vector<int> other);

  int operator* (const BinVec &) const;
  BinVec operator+ (const BinVec &) const;
  friend BinVec operator* (int bit, const BinVec & bit_vector);
  friend int dot (int bit, const BinVec & bit_vector);
  IndexProxy operator[] (unsigned long idx);
  int operator[] (unsigned long idx) const;
};

#endif
