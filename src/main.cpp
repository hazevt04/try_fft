// C++ File for main

#include "my_utils.hpp"
#include "my_file_io_funcs.hpp"

#include "cookbook_fft.hpp"
#include "ct_fft.hpp"

int main() {
   try {
      int num_samples_bits = 20;
      int num_samples = 1u << num_samples_bits;

      complex_vec<float> samples( num_samples );
      
      read_binary_complex_file<float>(samples,
         "../testdataBPSKcomplex.bin",
         num_samples,
         false);

      complex_vec<float> some_expected_frequencies{ 
         {-7440., 8288.}, {9708.656, 3208.545}, {548.88574, 3287.003},
         {-3839.7205, 4795.2656}, {5758.583, -7036.872}, {-3411.0234  -2247.5532},
         {4180.694, 4903.826}, {5899.282, 3323.6128}, {-5254.4717, -11024.913},
         {1275.5605, 3912.9788}
      };

      complex_vec<float> frequencies( num_samples );
      std::fill( frequencies.begin(), frequencies.end(), 0 );

      cookbook_fft(samples, frequencies, num_samples_bits);

      const char delim[] = " ";
      const char suffix[] = "\n";
      print_vals<std::complex<float>>(samples, "Cookbook FFT samples: ", delim, suffix);
      print_vals<std::complex<float>>(some_expected_frequencies, "Cookbook FFT some expected frequencies:\n", delim, suffix);
      print_vals<std::complex<float>>(frequencies.data(), (int)some_expected_frequencies.size(), 0, "Cookbook FFT frequencies:\n", delim, suffix);
         
      return EXIT_SUCCESS;

   } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}
