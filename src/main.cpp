// C++ File for main

#include "my_utils.hpp"

#include "my_fft.hpp"

int main(int argc, char** argv) {
   try {
      bool debug = true;

      typedef std::complex<double> cx;
      cx a[] = {cx(0, 0), cx(1, 1), cx(3, 3), cx(4, 4), cx(4, 4), cx(3, 3), cx(1, 1), cx(0, 0)};
      cx b[8];
      my_fft(a, b, 3);
      for (int i = 0; i < 8; ++i)
         std::cout << b[i] << "\n";

      return EXIT_SUCCESS;

   } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}