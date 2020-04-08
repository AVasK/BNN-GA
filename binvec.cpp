#include "binvec.hpp"
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <iostream>
#include <exception>
#include <stdexcept>

ITYPE mask_last(int mask_on_bits)
{
  return ~(ITYPE(-1) >> mask_on_bits << mask_on_bits);
}

ITYPE left_ith_bit(int index) // from left to right
{
  return ITYPE(1 << (ISIZE - index - 1));
}

ITYPE ith_bit(int index) // from left to right
{
  return ITYPE(1 << index);
}

inline unsigned pow_of_2 (int pow)
{
  return 1 << pow;
}

std::string to_binary(int num, int bits=ISIZE)
{
  std::string binary_string(bits, 'x');

  for ( int i = bits-1; i >= 0; i-- )
  {
    binary_string[i] = int(num & 1)+'0';
    num = num >> 1;
  }
  return binary_string;
}

char BinVec::size() const
{
  return num_of_bits;
}

void init_srand()
{
  static bool set = false;
  if (!set) {
    std::srand(std::time(0));
    set = true;
  }
}

BinVec::BinVec(int length)
{
  num_of_bits = length;
  num_of_int_containers = (length / int_container_size) + 1;

  // Init with random numbers
  init_srand();
  for ( int i = 0; i < num_of_int_containers; i++ )
  {
    bits.push_back(std::rand());
  }
}

BinVec::BinVec(int length, int bit)
{
  num_of_bits = length;
  num_of_int_containers = (length / int_container_size) + 1;

  auto num = 0;
  if (bit == 1) num = ITYPE(-1);
  // Zeroed out init
  for ( int i = 0; i < num_of_int_containers; i++ )
  {
    bits.push_back(num);
  }
}

std::ostream & operator<<(std::ostream & os, const BinVec & vec)
{
  os << "b[";
  for ( int i = 0; i < vec.num_of_int_containers - 1; i++ )
  {
      auto num = vec.bits[i];
      os << to_binary(num) << ",";
  }
  int i = vec.num_of_int_containers - 1;
  auto num = vec.bits[i];
  auto bits_num = vec.num_of_bits % vec.int_container_size;
  os << to_binary(num, bits_num)
     << "]";

  return os;
}


int BinVec::operator* (const BinVec & other) const
{
  if (num_of_bits != other.size())
  {
    throw std::length_error("Vectors in dot product should have equal length!");
  }

  // in binary vectors we can replace usual Multiply-Accumulate (MAC) operation
  // in dot products
  // with this formula:
  //    dot = 2*popcount( xnor(V1, V2) )
  //
  // That's a little bit trickier,
  // since in most papers they just say it's popcount(xnor), which it's not..

  int dot = 0;
  for (int i = 0; i < num_of_int_containers - 1; i++)
  {
    auto mask = mask_last(ISIZE);
    auto popcnt = __builtin_popcount(mask & ~(bits[i] ^ other.bits[i])); // popcount(xnor)
    dot += (popcnt << 1) - ISIZE;
  }
  int i = num_of_int_containers - 1;
  auto bits_num = num_of_bits % int_container_size;
  auto mask = mask_last(bits_num);
  auto popcnt = __builtin_popcount(mask & ~(bits[i] ^ other.bits[i]));
  dot += (popcnt << 1) - bits_num;

  return dot;
}

int operator* (int bit, const BinVec & bit_vector)
{
  auto num_of_int_containers = bit_vector.num_of_int_containers;
  auto bits = bit_vector.bits;
  auto num_of_bits = bit_vector.num_of_bits;
  auto int_container_size = bit_vector.int_container_size;

  int dot = 0;
  auto num_binary = 0; // set all-zeros
  if (bit == 1) { num_binary = ITYPE(-1); } // set all-ones

  for (int i = 0; i < num_of_int_containers - 1; i++)
  {
    auto mask = mask_last(ISIZE);
    auto popcnt = __builtin_popcount(mask & ~(bits[i] ^ num_binary)); // popcount(xnor)
    dot += (popcnt << 1) - ISIZE;
  }
  int i = num_of_int_containers - 1;
  auto bits_num = num_of_bits % int_container_size;
  auto mask = mask_last(bits_num);
  auto popcnt = __builtin_popcount(mask & ~(bits[i] ^ num_binary));
  dot += (popcnt << 1) - bits_num;

  return dot;
}

int BinVec::get_bit(int index) const
{
  auto container_index = int(index / int_container_size);
  auto container_offset = index % int_container_size;

  if (
      (container_index >= num_of_int_containers)
       ||
      (container_offset >= num_of_bits % int_container_size)
     )
  {
    throw std::out_of_range("index out of range");
  }

  auto container = bits[container_index];
  auto bitmask = ith_bit(container_offset);
  return int(int(container & bitmask) > 0);
}


void BinVec::set_bit(int value, int index)
{
  auto container_index = int(index / int_container_size);
  auto container_offset = index % int_container_size;

  if (
      (container_index >= num_of_int_containers)
       ||
      (container_offset >= num_of_bits % int_container_size)
     )
  {
    throw std::out_of_range("index out of range");
  }

  auto& container = bits[container_index];
  auto bitmask = ith_bit(container_offset);
  if ( value == 0 ) // unset bit
  {
    bitmask = ~bitmask;
    container = container & bitmask;
  }
  // set bit
  else
  container = container | bitmask;
}

BinVec::IndexProxy BinVec::operator[] (int idx)
{
  return IndexProxy(*this, idx);
}

void BinVec::IndexProxy::operator= (int value)
{
  ref.set_bit(value, idx);
}

BinVec::IndexProxy::operator int() const
{
  return ref.get_bit(idx);
}
