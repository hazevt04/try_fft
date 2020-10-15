// C++ File for main

#include "my_utils.hpp"
#include "my_file_io_funcs.hpp"

#include "cookbook_fft.hpp"
#include "ct_fft.hpp"

#include "expected_frequencies.hpp"

int main() {
   try {
      const bool debug = true;
      unsigned int num_samples_bits = 6;
      int num_samples = (1u << num_samples_bits);

      complex_vec<float> samples( num_samples );
      
      read_binary_complex_file<float>(samples,
         "../testdataBPSKcomplex.bin",
         num_samples,
         false);

      complex_vec<float> frequencies( num_samples );
      std::fill( frequencies.begin(), frequencies.end(), 0 );

      Time_Point start = Steady_Clock::now();
      
      cookbook_fft(samples, frequencies, num_samples_bits);
      
      Duration_ms duration_ms = Steady_Clock::now() - start;
      
      const float max_diff = 1e-2;
      auto check_results = complex_vecs_close( frequencies, expected_frequencies, max_diff );
      if ( false == check_results.first ) {
         throw std::runtime_error{ std::string{"Mismatch: "} +
            std::string{"Index: "} + std::to_string(check_results.second) + 
            std::string{" Expected Frequency: {"} + 
               std::to_string(expected_frequencies.at(check_results.second).real()) + ", " +
               std::to_string(expected_frequencies.at(check_results.second).imag()) + 
               "}" +
            std::string{" Actual Frequency: {"} + 
               std::to_string(expected_frequencies.at(check_results.second).real()) + ", " +
               std::to_string(expected_frequencies.at(check_results.second).imag()) + 
               "}"
         };
      }
      std::cout << "All frequencies matched expected ( exponential calculation).\n"; 
      std::cout << "Cookbook FFT (with exponential calculation) with " << num_samples << " samples took " << duration_ms.count() << " milliseconds\n"; 
      
      // Try out the lookup version
      std::fill( frequencies.begin(), frequencies.end(), 0 );

      start = Steady_Clock::now();
      
      cookbook_fft_lookup(samples, frequencies, num_samples_bits);
      
      duration_ms = Steady_Clock::now() - start;
      
      check_results = complex_vecs_close( frequencies, expected_frequencies, max_diff );
      if ( false == check_results.first ) {
         throw std::runtime_error{ std::string{"Mismatch: "} +
            std::string{"Index: "} + std::to_string(check_results.second) + 
            std::string{" Expected Frequency: {"} + 
               std::to_string(expected_frequencies.at(check_results.second).real()) + ", " +
               std::to_string(expected_frequencies.at(check_results.second).imag()) + 
               "}" +
            std::string{" Actual Frequency: {"} + 
               std::to_string(expected_frequencies.at(check_results.second).real()) + ", " +
               std::to_string(expected_frequencies.at(check_results.second).imag()) + 
               "}"
         };
      }
      std::cout << "All frequencies matched expected (with lookup).\n"; 
      std::cout << "Cookbook FFT (with lookup) with " << num_samples << " samples took " << duration_ms.count() << " milliseconds\n"; 
     
      //cookbook_fft_debug(samples, frequencies, num_samples_bits);
      //if ( debug ) {
      //   const char delim[] = ", ";
      //   const char suffix[] = "\n";
      //   print_vals<std::complex<float>>(samples.data(), num_samples, "Samples: ", delim, suffix);
      //   print_vals<std::complex<float>>(expected_frequencies.data(), num_samples, "Expected frequencies: ", delim, suffix);
      //   print_vals<std::complex<float>>(frequencies.data(), num_samples, "Actual frequencies: ", delim, suffix);
      //}
      return EXIT_SUCCESS;

   } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}
