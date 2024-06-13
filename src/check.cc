#include <filesystem>
#include <string>
#include "check.h"

bool check_file(const std::string& file){
  return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);
}

bool check_dir(const std::string& dir){
  return std::filesystem::exists(dir) && std::filesystem::is_regular_file(dir);
}
