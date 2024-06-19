#include <string>
#include <filesystem>

#include "check.h"

bool check_is_file(const std::string& file){
  return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);
}

bool check_is_dir(const std::string& dir){
  return std::filesystem::exists(dir) && std::filesystem::is_directory(dir);
}

