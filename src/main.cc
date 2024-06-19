//==========================================================| LIBS

#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <libconfig.h++>
#include <filesystem>

//==========================================================| MY HEARDERS

#include "check.h"

//==========================================================| MACROS

#define RED "\033[031m"
#define GREEN "\033[032m"
#define YELLOW "\033[033m"
#define NC "\033[0m"

//==========================================================| CONST VARS

const std::string config_file = "/etc/lala.conf";
const std::string repo_dir = "/var/lalapkg/repo";
const std::string build_file = "Buildpkg";
const std::string world_dir = "/var/lalapkg/world/";

//==========================================================| PACKAGE_VECTOR

std::vector<std::string> packages_vector;

//==========================================================| CONFIG_FILE STRUCT 

struct Config_file{
  std::string sync;
  std::string source_dir;
  std::string root_dir;
  std::string installbin_dir;
  std::string custom_repo;
  std::string common_flags;
  std::string jobs;
};

Config_file conf_file;

//==========================================================| FUNCTIONS

int load_config(const std::string& file){
  libconfig::Config conf_obj;
  
  try{
    conf_obj.readFile(file);
    
    const std::string var_names[] = {"sync", "source_dir", "root_dir", "installbin_dir", "custom_repo", "common_flags", "jobs"};
    std::string* var_pts[] = {&conf_file.sync, &conf_file.source_dir, &conf_file.root_dir, &conf_file.installbin_dir, &conf_file.custom_repo, &conf_file.common_flags, &conf_file.jobs};
    
    const size_t size = sizeof(var_pts) / sizeof(var_pts[0]);

    bool ERROR = false;

    std::cout << GREEN << "|==========VAR-RESULTS=========|" << NC << std::endl;
    
    for(int i = 0; i < size; i++){
      conf_obj.lookupValue(var_names[i], *var_pts[i]);
      if(var_pts[i]->empty()){
        if(var_names[i] == "common_flags" || var_names[i] == "jobs"){
          std::cerr << var_names[i] << "...   [ " << YELLOW << "WARNING" << NC << " ]" << std::endl;
        } else {
          ERROR = true;
          std::cerr << var_names[i] << "...   [ " << RED << "FAILED" << NC << " ]" << std::endl;
        }
      } else {
        std::cerr << var_names[i] << "...   [ " << GREEN << "OK"<< NC << " ]" << std::endl;
      }
    }

    std::cout << GREEN << "|==============================|" << NC << std::endl;

    if(ERROR){
      throw std::runtime_error("Error while trying to load some var: Check config file -> " GREEN + config_file + NC);
    }
    return EXIT_SUCCESS;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch(libconfig::ParseException &paex){
    std::cerr << RED << "ERROR: " << NC <<  "Parse error in build file -> " << GREEN << paex.getFile() << NC <<" -> " << RED << paex.getError() << NC << " ->" << " line -> " << RED << paex.getLine() << NC << std::endl;
    return EXIT_FAILURE;
  }

  catch(libconfig::FileIOException &fioex){
    std::cerr << RED << "ERROR: " << NC << "I/O error reading build file ->" << RED << fioex.what() << NC << std::endl;
    return EXIT_FAILURE;
  }
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

void put_package_in_vector(std::string& package){ 
  packages_vector.insert(packages_vector.begin() + packages_vector.size(), package);
}

char parse_arguments(char* arg[], int& num_args){
  char user_arg = 'n';
  int index = 1;
  
  for(int i = 0; i < num_args; i++){
    switch(arg[i][1]){
      case 'e': 
        user_arg = 'e';
        index += i;
        
        while(arg[index] != nullptr){
          std::string package(arg[index]);
          put_package_in_vector(package);
          index++;
        }
      break;

      case 'u':
        user_arg = 'u';
      break;
    }

    if(user_arg != 'n'){
      break;
    }
  }
  return user_arg;
}

//==========================================================| MAIN

int main(int argc, char* argv[]){
  const std::string* check_this_dirs[] = {&conf_file.source_dir, &conf_file.root_dir, &conf_file.installbin_dir, &conf_file.custom_repo, &repo_dir, &world_dir}; 
  const size_t size = sizeof(check_this_dirs) / sizeof(check_this_dirs[0]);

  if(load_config(config_file) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(check_dirs(check_this_dirs, repo_dir, size) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  const char arg = parse_arguments(argv, argc);

  switch(arg){
    case 'e':
      std::cout << "Instalar esses pacotes: " << std::endl;
      for(int i = 0; i < packages_vector.size(); i++){
        std::cout << packages_vector[i] << std::endl;
      }
    break;

    case 'u':
    break;
  }

  return 0;
}
