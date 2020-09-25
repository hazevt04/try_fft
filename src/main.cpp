// C++ File for main

#include "my_utils.hpp"

#include "cookbook_fft.hpp"
#include "ct_fft.hpp"

#ifdef CORRECTNESS_CHECK

int main() {
   try {
      int num_samples_bits = 3;
      int num_samples = static_cast<int>( 1u << num_samples_bits );

      complex_vec<double> samples{ {0, 0}, {1, 1}, {3, 3}, {4, 4}, 
         {4, 4}, {3, 3}, {1, 1}, {0, 0} };
      complex_vec<double> expected_frequencies{ {16,16}, {-4.82843,-11.6569}, {0,0}, 
         {-0.343146,0.828427}, {0,0}, {0.828427,-0.343146}, {0,0}, {-11.6569,-4.82843} };

      complex_vec<double> frequencies( num_samples );
      std::fill( frequencies.begin(), frequencies.end(), 0 );

      std::vector< std::pair< complex_vec<double>, complex_vec<double> > > tests{ 
         {
            { {0, 0}, {1, 1}, {3, 3}, {4, 4}, {4, 4}, {3, 3}, {1, 1}, {0, 0} },
            { {-11.6569,-4.82843}, {0,0}, {0.828427,-0.343146}, {0,0}, {-0.343146,0.828427}, {0,0}, {-4.82843,-11.6569}, {16,16} }
         }
      };

      for( auto test: tests ) {
         cookbook_fft(test.first, frequencies, num_samples_bits);

         const char delim[] = " ";
         const char suffix[] = "\n";
         print_vals<std::complex<double>>(test.first, "Cookbook FFT samples: ", delim, suffix);
         print_vals<std::complex<double>>(test.second, "Cookbook FFT expected frequencies:\n", delim, suffix);
         print_vals<std::complex<double>>(frequencies, "Cookbook FFT frequencies:\n", delim, suffix);
         
         auto check = complex_vecs_close<double>( frequencies, test.second, 1e-4 );
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

#else

#include <benchmark/benchmark.h>

static void BM_cookbook_fft( benchmark::State& state ) {
   for ( auto _ : state ) {
      int log2n = state.range(0);
      unsigned int fft_size = 1 << log2n;
      complex_vec<double> a(fft_size);
      complex_vec<double> b(fft_size);
      
      benchmark::DoNotOptimize(a.data());
      benchmark::DoNotOptimize(b.data());
      benchmark::ClobberMemory();

      const std::complex<double> J(0, 1);
      for (unsigned int i = 0; i != fft_size; ++i) {
         unsigned int br_index = bit_reverse(i, log2n);
         b[br_index] = a[i];
      }
      
      for (int s = 1; s <= log2n; ++s) {
         unsigned int m = 1 << s;
         unsigned int m2 = m >> 1;
         std::complex<double> w(1, 0);
         std::complex<double> wm = exp(-J * (PI / m2));
         for (unsigned int j = 0; j != m2; ++j) {
            for (int k = j; k < (int)fft_size; k += m) {
               std::complex<double> t = w * b[k + m2];
               std::complex<double> u = b[k];
               b[k] = u + t;
               b[k + m2] = u - t;
            }
            w *= wm;
         }
      } // end of for (int s = 1; s <= log2n; ++s) {

      for(int index = 0; index < fft_size>>1; ++index) {
         std::complex<double> temp = b[fft_size-index-1];
         b[fft_size-index-1] = b[index];
         b[index] = temp;
      } 
    } // for ( auto _ : state ) {
}
BENCHMARK(BM_cookbook_fft)->Arg(13)->Arg(14)->Arg(15)->Arg(16)->Arg(17)->Arg(18)->Arg(19)->Arg(20);

BENCHMARK_MAIN();

#endif
