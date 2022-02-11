#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Knuth shuffle
uint64_t shuffle() {
  uint64_t ret = 0x0123456789ABCDEF;
  for (size_t i = 0x10; i > 0; --i) {
    size_t j = 4 * (rand() % i);
    size_t k = 4 * (i - 1);
    if (j == k) { continue; }
    ret ^= ((ret >> k) & 0xF) << j; // ret[j] ^= ret[k];
    ret ^= ((ret >> j) & 0xF) << k; // ret[k] ^= ret[j];
    ret ^= ((ret >> k) & 0xF) << j; // ret[j] ^= ret[k];
  }
  return ret;
}

// substract their neighbours and take absolute values
uint64_t abs01(uint64_t x) {
  uint64_t evn = x;
  evn &= 0x0F0F0F0F0F0F0F0F;

  // make them negative
  evn ^= 0x1F1F1F1F1F1F1F1F; 
  evn += 0x0101010101010101;

  uint64_t odd = x >> 4;
  odd &= 0x0F0F0F0F0F0F0F0F;

  // substract by adding negative numbers
  uint64_t sum = odd + evn;

  // compose the mask
  uint64_t msk = sum >> 4;
  msk &= 0x0101010101010101;
  msk ^= 0x0101010101010101;
  msk += 0x1F1F1F1F1F1F1F1F;

  // take absolute values
  sum ^= msk;
  sum += 0x0101010101010101 & msk;
  sum &= 0x0F0F0F0F0F0F0F0F;
  return sum;
}

// substract their neighbours (another side) and take absolute values
uint64_t abs12(uint64_t x) {
  uint64_t evn = x;
  evn &= 0x0F0F0F0F0F0F0F00;

  // make them negative
  evn ^= 0x1F1F1F1F1F1F1F00;
  evn += 0x0101010101010100;

  // substract by adding negative numbers
  uint64_t odd = x << 4;
  odd &= 0x0F0F0F0F0F0F0F00;
  
  // substract by adding negative numbers
  uint64_t sum = odd + evn;
  
  // compose the mask
  uint64_t msk = sum >> 4;
  msk &= 0x0101010101010100;
  msk ^= 0x0101010101010100;
  msk += 0x1F1F1F1F1F1F1F00;
  
  // take absolute values
  sum ^= msk;
  sum += 0x0101010101010100 & msk;
  sum &= 0x0F0F0F0F0F0F0F00;
  return sum;
}

// add their neighbours
uint64_t sum01(uint64_t x) {
  uint64_t evn = x;
  evn &= 0x0F0F0F0F0F0F0F0F;
  uint64_t odd = x >> 4;
  odd &= 0x0F0F0F0F0F0F0F0F;
  return odd + evn;
}

// add their neighbours (another side)
uint64_t sum12(uint64_t x) {
  uint64_t evn = x;
  evn &= 0x0F0F0F0F0F0F0F00;
  uint64_t odd = x << 4;
  odd &= 0x0F0F0F0F0F0F0F00;
  return odd + evn;
}

// exchange with their neighbours if necessary
uint64_t exchange01(uint64_t x) {
  uint64_t sum = sum01(x);
  uint64_t abs = abs01(x);

  // get -abs
  uint64_t mus = abs;
  mus ^= 0x1F1F1F1F1F1F1F1F;
  mus += 0x0101010101010101;
  
  // get smaller ones
  uint64_t sma = ((sum + mus) >> 1) << 4;
  sma &= 0xF0F0F0F0F0F0F0F0;

  // get larger ones
  uint64_t big = ((sum + abs) >> 1);
  big &= 0x0F0F0F0F0F0F0F0F;

  // combine them to one number
  return sma | big;
}

// exchange with their neighbours if necessary (another side)
uint64_t exchange12(uint64_t x) {
  uint64_t sum = sum12(x);
  uint64_t abs = abs12(x);

  // get -abs
  uint64_t mus = abs;
  mus ^= 0x1F1F1F1F1F1F1F00;
  mus += 0x0101010101010100;

  // get smaller ones
  uint64_t sma = ((sum + mus) >> 1);
  sma &= 0x0F0F0F0F0F0F0F00;

  // get larger ones
  uint64_t big = ((sum + abs) >> 5);
  big &= 0x00F0F0F0F0F0F0F0;

  // combine them to one number
  uint64_t ret = sma | big;
  
  // recover the first and the last number, which are not involved
  ret |= 0xF00000000000000F & x;
  return ret;
}

// odd-even transposition sort
uint64_t sort(uint64_t x) {
  uint64_t ret = x;
  for (size_t i = 0; i < 0x10; ++i) {
    if (i % 2 == 0) {
      ret = exchange01(ret);
    } else {
      ret = exchange12(ret);
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  for (size_t i = 0; i < 1; ++i) {
    uint64_t src = shuffle();
    uint64_t dst = sort(src);
    printf("%016llX -> %016llX\n", src, dst);
    assert(dst == 0x0123456789ABCDEF);
  }
  return 0;
}