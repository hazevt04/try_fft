// C++ File for main

#include "my_utils.hpp"

#include "cookbook_fft.hpp"
#include "ct_fft.hpp"

int main(int argc, char** argv) {
   try {
      bool debug = true;

      typedef std::complex<double> cx;
      cx a[] = {cx(0, 0), cx(1, 1), cx(3, 3), cx(4, 4), 
         cx(4, 4), cx(3, 3), cx(1, 1), cx(0, 0)};
      cx b[8];
      cookbook_fft(a, b, 3);
      for (int i = 0; i < 8; ++i)
         std::cout << b[i] << "\n";

      // double ct_a[] = {0, 0, 1, 1, 3, 3, 4, 4,
         // 4, 4, 3, 3, 1, 1, 0, 0};
      // ct_fft(ct_a, 8);
      // for (int i = 0; i < 16; ++i)
         // std::cout << ct_a[i] << "\n";

      return EXIT_SUCCESS;

   } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}
