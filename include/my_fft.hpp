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


#include <utility>
#include <cmath>
// Classical Cooley-Tukey
// https://www.drdobbs.com/cpp/a-simple-and-efficient-fft-implementatio/199500857#
void ct_fft(double* data, unsigned long nn)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
 
    // reverse-binary reindexing
    n = nn<<1;
    j=1;
    for (i=1; i<n; i+=2) {
        if (j>i) {
            std::swap(data[j-1], data[i-1]);
            std::swap(data[j], data[i]);
        }
        m = nn;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };
 
    // here begins the Danielson-Lanczos section
    mmax=2;
    while (n>mmax) {
        istep = mmax<<1;
        theta = -(2*M_PI/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m=1; m < mmax; m += 2) {
            for (i=m; i <= n; i += istep) {
                j=i+mmax;
                tempr = wr*data[j-1] - wi*data[j];
                tempi = wr * data[j] + wi*data[j-1];
 
                data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp=wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax=istep;
    }
}
