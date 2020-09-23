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


void test_my_file_io_funcs(
   std::string filename, const int num_vals, const bool inject_error, const bool debug);
