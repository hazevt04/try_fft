#pragma once

#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>

#include "my_utils.hpp"

// FFT Implementation from:
// https://www.oreilly.com/library/view/c-cookbook/0596007612/ch11s18.html#cplusplusckbk-CHP-11-EX-33

unsigned int bit_reverse(unsigned int x, int log2n) {
   int n = 0;
   int mask = 0x1;
   for (int i = 0; i < log2n; i++) {
      n <<= 1;
      n |= (x & 1);
      x >>= 1;
   }
   return n;
}

constexpr double PI = 3.1415926536;

template <class Iter_T>
void my_fft(Iter_T a, Iter_T b, int log2n) {

   typedef typename std::iterator_traits<Iter_T>::value_type complex;
   const complex J(0, 1);
   int n = 1 << log2n;
   for (unsigned int i = 0; i < n; ++i) {
      b[bit_reverse(i, log2n)] = a[i];
   }
   for (int s = 1; s <= log2n; ++s) {
      int m = 1 << s;
      int m2 = m >> 1;
      complex w(1, 0);
      complex wm = exp(-J * (PI / m2));
      for (int j = 0; j < m2; ++j) {
         for (int k = j; k < n; k += m) {
            complex t = w * b[k + m2];
            complex u = b[k];
            b[k] = u + t;
            b[k + m2] = u - t;
         }
         w *= wm;
      }
   }
}
