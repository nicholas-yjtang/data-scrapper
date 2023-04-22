#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <boost/log/trivial.hpp>

string Utils::readFile(const string & filename) {
  ifstream t(filename);
  stringstream buffer;
  buffer << t.rdbuf();  
  return buffer.str();
}

void Utils::closeThread(shared_ptr<thread> thread) {
  if (thread != nullptr) thread->join();
}

bool Utils::compareFiles(const std::string& file1, const std::string& file2) {
  string temp_file1 = file1;
  string temp_file2 = file2;
  if (temp_file1.find("file:///") == 0) temp_file1 = temp_file1.substr(8);
  if (temp_file2.find("file:///") == 0) temp_file2 = temp_file2.substr(8);
  BOOST_LOG_TRIVIAL(debug) << "Comparing files: " << temp_file1 << " and " << temp_file2;
  std::ifstream filestream1(temp_file1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream filestream2(temp_file2, std::ifstream::binary|std::ifstream::ate);

  if (filestream1.fail() || filestream2.fail()) {
    BOOST_LOG_TRIVIAL(debug) << "Failed to open files: " << temp_file1 << " and " << temp_file2;
    return false; //file problem
  }

  if (filestream1.tellg() != filestream2.tellg()) {
    BOOST_LOG_TRIVIAL(debug) << "File sizes do not match";
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  filestream1.seekg(0, std::ifstream::beg);
  filestream2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(filestream1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(filestream2.rdbuf()));
}