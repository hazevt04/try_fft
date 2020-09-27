// C++ File for main
#include <tuple>
#include "my_utils.hpp"

#include "cookbook_fft.hpp"
#include "ct_fft.hpp"

int main() {
   try {

      std::vector< std::pair<complex_vec<float>, complex_vec<float> >  > tests{ 
         {
               { {0, 0}, {1, 1}, {3, 3}, {4, 4}, {4, 4}, {3, 3}, {1, 1}, {0, 0} },
               { {16,16}, {-4.82843,-11.6569}, {0,0}, {-0.343146,0.828427}, {0,0}, {0.828427,-0.343146}, {0,0}, {-11.6569,-4.82843} }
         }
      };

      for( auto test: tests ) {
         int num_samples = (int)test.first.size();
         int num_samples_bits = 3;
         
         complex_vec<float> frequencies(num_samples);
         
         cookbook_fft(test.first, frequencies, num_samples_bits);

         const char delim[] = " ";
         const char suffix[] = "\n";
         print_vals<std::complex<float>>(test.first, "Cookbook FFT samples: ", delim, suffix);
         print_vals<std::complex<float>>(frequencies, "Cookbook FFT frequencies:\n", delim, suffix);
         
         auto check = complex_vecs_close<float>( frequencies, test.second, 1e-4 );
         if ( check.second >= 0 ) {
            std::cout << "ERROR: Item " << check.second << " actual: " 
               << frequencies[check.second] << " is too far from the expected: "
               << test.second[check.second] << "\n"; 
         }
         std::cout << "Test PASSED\n"; 
         std::fill( frequencies.begin(), frequencies.end(), 0 );
      }
      return EXIT_SUCCESS;

   } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}
