//==========================================|
//   FILE: check.cc                         |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <cstdlib>
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>

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

int check_argument(char* arg[], int& num_args, char user_arg[], std::vector<std::string>& packages_vector){
  user_arg[0] = '\0';

  for(int i = 0; i < num_args; i++){
    if(arg[i][0] == '-'){
      switch(arg[i][1]){
        case 'e':
          user_arg[0] = 'e';

          while(arg[++i] != nullptr){
            packages_vector.push_back(arg[i]);
          }
        break;

        case 'u':
          user_arg[0] = 'u';

          while(arg[++i] != nullptr){
            packages_vector.push_back(arg[i]);
          }
        break;

        case 'i':
         user_arg[0] = 'i';
         user_arg[1] = arg[i][2];

          if(arg[i + 1] != nullptr){
            packages_vector.push_back(arg[++i]);
          }
        break;

        default:
          std::cerr << RED << "ERROR: " << NC << "Invalid argument: " << GREEN << arg[i] << NC << std::endl;  
          return EXIT_FAILURE;
        break;
      }
    }
  }

  if(user_arg[0] == '\0'){
    std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "argument" << NC << std::endl;
    return EXIT_FAILURE;
  }

  if(packages_vector.empty()){
    std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "package" << NC << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
