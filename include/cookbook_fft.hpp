#pragma once

#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>

#include "my_utils.hpp"

// FFT Implementation from C++ Cookbook:
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

//template <class Iter_T>
//void cookbook_fft(Iter_T a, Iter_T b, int log2n) {

   //typedef typename std::iterator_traits<Iter_T>::value_type complex;
   //const complex J(0, 1);
   //int fft_size = 1 << log2n;
   
   //for (unsigned int i = 0; i < fft_size; ++i) {
      //unsigned int br_index = bit_reverse(i, log2n);
      //b[br_index] = a[i];
   //}

   //for (int s = 1; s <= log2n; ++s) {
      //int m = 1 << s;
      //int m2 = m >> 1;
      //complex w(1, 0);
      //complex wm = exp(-J * (PI / m2));
      //for (int j = 0; j < m2; ++j) {
         //for (int k = j; k < fft_size; k += m) {
            //complex t = w * b[k + m2];
            //complex u = b[k];
            //b[k] = u + t;
            //b[k + m2] = u - t;
         //}
         //w *= wm;
      //}
   //}
//}


template <typename T>
void cookbook_fft(complex_vec<T>& a, complex_vec<T>& b, int log2n) {

   complex_vec<T> t_b(b.size(), 0);
   const std::complex<T> J(0, 1);
   unsigned int fft_size = 1 << log2n;
   
   for (unsigned int i = 0; i != fft_size; ++i) {
      unsigned int br_index = bit_reverse(i, log2n);
      t_b[br_index] = a[i];
   }

   for (int s = 1; s <= log2n; ++s) {
      unsigned int m = 1 << s;
      unsigned int m2 = m >> 1;
      std::complex<T> w(1, 0);
      std::complex<T> wm = exp(-J * (T(PI) / m2));
      for (unsigned int j = 0; j != m2; ++j) {
         for (int k = j; k < (int)fft_size; k += m) {
            std::complex<T> t = w * t_b[k + m2];
            std::complex<T> u = t_b[k];
            t_b[k] = u + t;
            t_b[k + m2] = u - t;
         }
         w *= wm;
      }
   } // end of for (int s = 1; s <= log2n; ++s) {
      
   // Swap the halves
   // "FFT Shift"
   unsigned int half_fft_size = fft_size >> 1;
   for(int index = 0; index < fft_size; ++index) {
      if ( index < (int)half_fft_size ) {
        b[index] = t_b[index + half_fft_size]; 
      } else {
        b[index] = t_b[index - half_fft_size]; 
      }
   }
}

#include "ws.hpp"

template <typename T>
void cookbook_fft_lookup(complex_vec<T>& a, complex_vec<T>& b, int log2n) {

   complex_vec<T> t_b(b.size(), 0);
   const std::complex<T> J(0, 1);
   unsigned int fft_size = 1 << log2n;
   
   for (unsigned int i = 0; i != fft_size; ++i) {
      unsigned int br_index = bit_reverse(i, log2n);
      t_b[br_index] = a[i];
   }

   int ws_index = 0;
   for (int s = 1; s <= log2n; ++s) {
      unsigned int m = 1 << s;
      unsigned int m2 = m >> 1;
      std::complex<T> w(1, 0);
      for (unsigned int j = 0; j != m2; ++j) {
         for (int k = j; k < (int)fft_size; k += m) {
            // Replaced calculation of wm 
            // (with complex exponentiation) 
            // and w multiplication with a single table lookup!
            std::complex<T> t = ws[ws_index++] * t_b[k + m2];
            std::complex<T> u = t_b[k];
            t_b[k] = u + t;
            t_b[k + m2] = u - t;
         }
      }
   } // end of for (int s = 1; s <= log2n; ++s) {
      
   // Swap the halves
   // "FFT Shift"
   unsigned int half_fft_size = fft_size >> 1;
   for(int index = 0; index < fft_size; ++index) {
      if ( index < (int)half_fft_size ) {
        b[index] = t_b[index + half_fft_size]; 
      } else {
        b[index] = t_b[index - half_fft_size]; 
      }
   }
}

template <typename T>
void cookbook_fft_debug(complex_vec<T>& a, complex_vec<T>& b, int log2n) {

   complex_vec<T> t_b(b.size(), 0);
   const std::complex<T> J(0, 1);
   int fft_size = 1 << log2n;
   
   std::cout << "Number of bits in FFT Size is " << log2n << "\n"; 
   std::cout << "FFT Size is " << fft_size << "\n"; 

   for (unsigned int i = 0; i < fft_size; ++i) {
      unsigned int br_index = bit_reverse(i, log2n);
      std::cout << "\t" << i << " bit reversed is " << br_index << "\n"; 
      t_b[br_index] = a[i];
   }

   for (int s = 1; s <= log2n; ++s) {
      int m = 1 << s;
      int m2 = m >> 1;

      std::cout << "\ts = " << s << " m is " << m << "\n"; 
      std::cout << "\ts = " << s << " m2 is " << m2 << "\n"; 
      std::complex<T> w(1, 0);
      std::complex<T> wm = exp(-J * (T(PI) / m2));
      std::cout << "\t-J " << -J << "\n"; 
      std::cout << "\ts = " << s << " wm, the exponent, is " << wm << "\n"; 
      for (int j = 0; j < m2; ++j) {
         std::cout << "\ts is " << s << " and j is " << j << "\n"; 
         for (int k = j; k < fft_size; k += m) {
            
            std::cout << "\t\ts = " << s << " and j is " << j << " and k, one index into b, is " << k << "\n"; 
            std::cout << "\t\ts = " << s << " and j is " << j << " and m2 is " << m2 << "\n"; 
            std::cout << "\t\ts = " << s << " and j is " << j << " and k + m2, the other index into t_b, is " 
               << (k + m2) << "\n"; 
            std::cout << "\t\ts = " << s << " and j is " << j << " and w is " << w << "\n"; 

            std::complex<T> t = w * t_b[k + m2];
            std::complex<T> u = t_b[k];
            t_b[k] = u + t;
            t_b[k + m2] = u - t;
            std::cout << "\t\ts = " << s << " and j is " << j <<  " and t_b[" << k << "] = " << t_b[k] << "\n";
            std::cout << "\t\ts = " << s << " and j is " << j <<  " and t_b[" << k + m2 << "] = " << t_b[k + m2] 
               << "\n\n";
         }
         w *= wm;
         std::cout << "\ts = " << s << " j = " << j << ": w = " << w << "\n\n"; 
      } // end of for (int j = 0; j < m2; ++j) {
   } // end of for (int s = 1; s <= log2n; ++s) {
   
   // Swap the halves
   // "FFT Shift"
   unsigned int half_fft_size = fft_size >> 1;
   for(int index = 0; index < (int)fft_size; ++index) {
      if ( index < half_fft_size ) {
        b[index] = t_b[index + half_fft_size]; 
      } else {
        b[index] = t_b[index - half_fft_size]; 
      }
   }


}



