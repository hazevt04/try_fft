// C++ File for my_file_io_funcs

#include "my_file_io_funcs.hpp"
#include "my_utils.hpp"


void check_num_file_bytes(llong& num_file_bytes, const char* filename, const bool debug = false) {

   try {
      std::ifstream ifile;
      ifile.open(filename, std::ios::in | std::ios::binary);
      if (ifile.is_open()) {
         // get length of file:
         ifile.seekg(0, ifile.end);
         num_file_bytes = (llong)ifile.tellg();
         ifile.seekg(0, ifile.beg);
         debug_cout(
            debug, __func__, "(): File size for ", filename, " is ", num_file_bytes, " bytes\n\n");
      } else {
         throw std::runtime_error{std::string{"Unable to open file, "} + filename +
            std::string{", for checking filesize."}};
      }
   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   } // end of catch

} // end of void check_num_file_bytes()


void write_binary_floats_file(
   float* vals, const char* filename, const int num_vals, const bool debug = false) {

   try {
      std::ofstream ofile;
      int val_num = 0;
      std::streampos num_file_bytes;
      ofile.open(filename, std::ios::out | std::ios::binary);
      if (ofile.is_open()) {
         std::streamsize num_val_bytes = num_vals * sizeof(float);
         debug_cout(debug, __func__, "(): Val size is ", num_val_bytes, " bytes\n\n");
         ofile.write((char*)vals, num_val_bytes);
      } else {
         throw std::runtime_error{
            std::string{"Unable to open file, "} + filename + std::string{", for writing."}};
      }
      ofile.close();

      if ((ofile.rdstate() & std::ofstream::failbit) != 0) {
         throw std::runtime_error{
            std::string{"Logical error while writing file, "} + filename + std::string{"."}};
      }
      if ((ofile.rdstate() & std::ofstream::badbit) != 0) {
         throw std::runtime_error{
            std::string{"Write error while writing file, "} + filename + std::string{"."}};
      }
   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   }
} // end of write_binary_floats_file()


void write_binary_complex_floats_file(std::vector<std::complex<float>>& vals,
   const std::string& filename,
   const int num_vals,
   const bool debug = false) {

   try {
      std::ofstream ofile;
      int val_num = 0;
      ofile.open(filename, std::ios::out | std::ios::binary);
      if (ofile.is_open()) {
         auto num_val_bytes = num_vals * sizeof(std::complex<float>);
         debug_cout(debug, __func__, "(): Val size is ", num_val_bytes, " bytes\n\n");
         ofile.write(reinterpret_cast<char*>(vals.data()), num_val_bytes);
      } else {
         throw std::runtime_error{
            std::string{"Unable to open file, "} + filename + std::string{", for writing."}};
      }

      if ((ofile.rdstate() & std::ofstream::failbit) != 0) {
         throw std::runtime_error{
            std::string{"Logical error while writing file, "} + filename + std::string{"."}};
      }
      if ((ofile.rdstate() & std::ofstream::badbit) != 0) {
         throw std::runtime_error{
            std::string{"Write error while writing file, "} + filename + std::string{"."}};
      }
   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   }
} // end of write_binary_complex_floats_file()


void read_binary_complex_floats_file(std::vector<std::complex<float>>& vals,
   const std::string& filename,
   const int num_vals,
   const bool debug = false) {

   try {
      std::ifstream ifile;
      int val_num = 0;
      ifile.open(filename, std::ios::in | std::ios::binary);
      if (ifile.is_open()) {
         size_t num_val_bytes = num_vals * sizeof(std::complex<float>);
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

void read_binary_floats_file(
   float* vals, const char* filename, const int num_vals, const bool debug = false) {

   try {
      std::ifstream ifile;
      int val_num = 0;
      std::streampos num_file_bytes;
      ifile.open(filename, std::ios::in | std::ios::binary);
      if (ifile.is_open()) {
         size_t num_val_bytes = num_vals * sizeof(float);
         debug_cout(debug, __func__, "(): Val size is ", num_val_bytes, "\n");
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
         ifile.read((char*)vals, num_file_bytes);

      } else {
         throw std::runtime_error{
            std::string{"Unable to open file: "} + filename + std::string{"."}};
      } // end of if ( ifile.is_open() ) {
   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   }
}


void test_my_file_io_funcs(
   std::string filename, const int num_vals, const bool inject_error, const bool debug) {
   try {
      llong num_file_bytes = 0;

      std::vector<float> write_vals(num_vals);
      std::vector<float> read_vals(num_vals);

      gen_vals<float>(write_vals, 0, num_vals);
      print_vals<float>(write_vals, "Write Vals:\n");

      debug_cout(debug, "The input text file is ", filename, "\n");

      write_binary_floats_file(write_vals.data(), filename.c_str(), num_vals, debug);

      check_num_file_bytes(num_file_bytes, filename.c_str(), debug);

      if (inject_error) {
         filename = "wrong_file.bin";
      }

      read_binary_floats_file(read_vals.data(), filename.c_str(), num_vals, debug);

      print_vals<float>(read_vals, "Read Vals:\n");

      int num_mismatches = 0;
      if (!compare_floats(read_vals.data(), write_vals.data(), num_vals)) {
         throw std::runtime_error{std::string{"Values read from "} + filename +
            std::string{" don't match values written."}};
      } else {
         std::cout << "All " << num_vals << " values read from " << filename
                   << " matched the values written\n";
      }

   } catch (std::exception& ex) {
      throw std::runtime_error{std::string{__func__} + std::string{"(): "} + ex.what()};
   }
}


// end of C++ file for my_file_io_funcs
