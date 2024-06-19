#include <cstdlib>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <stdexcept>

#define RED "\033[031m"
#define GREEN "\033[032m"
#define NC "\033[0m"

#include "package_operations.h"
#include "check.h"

std::string package_exist(const std::string &repo, std::string& package_name, const std::string &script_file_name){
  std::vector<std::string> repo_sub_dirs;

  try{
    if(!check_is_dir(repo)){
      throw std::runtime_error("repository directory -> " GREEN + repo + NC " does not exist, use"  GREEN " lalapkg --sync" NC);
    }

    for(const auto& sub_dirs : std::filesystem::directory_iterator(repo)){
      if(std::filesystem::is_directory(sub_dirs)) {
        std::string directory = sub_dirs.path().string();
        repo_sub_dirs.push_back(directory);
      }
    }

    for(const auto& dirs : repo_sub_dirs){
      if(check_is_dir(dirs + "/" + package_name)){
        if(check_is_file(dirs + "/" + package_name + "/" + script_file_name)){
          return dirs + "/" + package_name + "/" + script_file_name;
        } else {
          throw std::runtime_error("Script file not found in -> " GREEN + dirs + "/" + package_name + "/" + script_file_name + NC);
        }
      } else {
        throw std::runtime_error("Package -> " GREEN + package_name + NC " not found");
      }
    }
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return "";
  }
}
