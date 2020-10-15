#!/usr/bin/env python3

import cmath as cm

# Script for gen_indices

num_bits = 6
fft_size = 1 << num_bits

def gen_ws():
   # Empirically discovered value
   # It is possible to calculate this
   max_i = 192
   i = 0
   print("#pragma once\n\n")
   print("#include \"my_utils.hpp\"\n")
   print("const complex_vec<float> ws{")
   for s in range(1,num_bits+1):
      m = 1 << s
      m2 = m >> 1
      J = complex(0,1)
      w = complex(1,0)
      wm = cm.exp( -J * complex(cm.pi/m2) )
      for j in range(m2):
         for k in range(j,fft_size,m):
            print("\t{{{:.12f},{:.12f}}}{}".format(w.real, w.imag,
               ("", ",")[i < max_i-1]))
            i = i + 1
         w = w * wm
      # end of for j in range(m2):
   # end of for s in range(1,num_bits+1):
   print("};")




if __name__ == '__main__':
   gen_ws() 
