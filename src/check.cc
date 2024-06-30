//==========================================|
//   FILE: check.cc                         |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <string>
#include <filesystem>
#include <iostream>

#include "check.h"
#include "color.h"
#include "thread"
#include "chrono"

bool check_is_file(const std::string& file){
  return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);
}

bool check_is_dir(const std::string& dir){
  return std::filesystem::exists(dir) && std::filesystem::is_directory(dir);
}

int check_dirs(const std::string* dirs[], const std::string& warning_dir, const int num_dirs){
  for(int i = 0; i < num_dirs; i++){
    if(!check_is_dir(*dirs[i])){
      if(*dirs[i] == warning_dir){
        std::cerr << YELLOW << "WARNING: " << NC << "repository directory -> " << GREEN << *dirs[i] << NC << " does not exist, use" << GREEN << " lalapkg --sync" << NC << std::endl;
      } else {
        std::cerr << YELLOW << "WARNING: " << NC << "Directory -> " << GREEN << *dirs[i] << NC << " not found" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        try{
          std::filesystem::create_directories(*dirs[i]);
          std::cout << ">>> Created Directory -> " << GREEN << *dirs[i] << NC << std::endl;
        }

        catch(std::filesystem::filesystem_error &error){
          std::cerr << RED << "ERROR: " << NC << "Failed to create directory -> " << GREEN << *dirs[i] << ": " << NC << error.what() << std::endl;
          return EXIT_FAILURE;
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
