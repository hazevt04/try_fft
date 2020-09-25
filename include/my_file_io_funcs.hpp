#pragma once

#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

#include "my_utils.hpp"

typedef long long llong;

void check_num_file_bytes(llong& file_size, const char* filename, const bool debug);

void write_binary_complex_floats_file(std::vector<std::complex<float>>& vals,
   const std::string& filename,
   const int num_vals,
   const bool debug);

void read_binary_complex_floats_file(std::vector<std::complex<float>>& vals,
   const std::string& filename,
   const int num_vals,
   const bool debug);


void write_binary_floats_file(
   float* vals, const char* filename, const int num_vals, const bool debug);

void read_binary_floats_file(
   float* vals, const char* filename, const int num_vals, const bool debug);

template<typename T>
void read_binary_complex_file(std::vector<std::complex<T>>& vals,
   const std::string& filename,
   const int num_vals,
   const bool debug = false) {

   try {
      std::ifstream ifile;
      int val_num = 0;
      ifile.open(filename, std::ios::in | std::ios::binary);
      if (ifile.is_open()) {
         size_t num_val_bytes = num_vals * sizeof(std::complex<T>);
         debug_cout(debug, __func__, "(): Val size is ", num_val_bytes, " bytes\n");
         ifile.seekg(0, ifile.end);
         llong num_file_bytes = (llong)ifile.tellg();
         ifile.seekg(0, ifile.beg);
         debug_cout(debug, __func__, "(): File size is ", (llong)num_file_bytes, " bytes\n\n");
         if (num_file_bytes < num_val_bytes) {
            throw std::runtime_error{std::string{"Expected file size, "} +
               std::to_string(num_file_bytes) + std::string{" bytes, less than expected: "} +
               std::to_string(num_val_bytes) + std::string{" bytes, for file "} + filename +
               std::string{"."}};
         }
         ifile.read(reinterpret_cast<char*>(vals.data()), num_val_bytes);

      } else {
         throw std::runtime_error{
            std::string{"Unable to open file: "} + filename + std::string{"."}};
      } // end of if ( ifile.is_open() ) {
   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   }
}

void test_my_file_io_funcs(
   std::string filename, const int num_vals, const bool inject_error, const bool debug);
