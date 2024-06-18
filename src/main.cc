//==========================================================| LIBS
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <stdexcept>
#include <libconfig.h++>
//==========================================================| MY_LIBS
#include "check.h"
//==========================================================| MACROS
#define RED "\033[031m"
#define GREEN "\033[032m"
#define YELLOW "\033[033m"
#define NC "\033[0m"
//==========================================================| CONST VARS
const std::string config_file = "/etc/lala.conf";
const std::string repo_dir = "/var/lalapkg/repo/";
const std::string build_File = "Buildpkg";
const std::string world_dir = "/var/lalapkg/world/";
//==========================================================| CONFIG_FILE VARS
struct ConfigData{
  std::string sync;
  std::string source_dir;
  std::string root_dir;
  std::string installbin_dir;
  std::string custom_repo;
  std::string common_flags;
  std::string jobs;
};
//==========================================================| PACKAGE VARS
//std::string name;
//std::string version;
//std::string source_url;

ConfigData config_data;
//==========================================================| FUNCTIONS
int load_config_file(const std::string& config_file){
  libconfig::Config conf;

  try{
    if(!check_file(config_file)){
      throw std::runtime_error("file -> " GREEN + config_file + NC " not found!");
    }

    conf.readFile(config_file);
    
    std::string vars_name[] = {"sync", "source_dir", "root_dir", "installbin_dir", "custom_repo", "common_flags", "jobs"};
    std::string* vars[] = {&config_data.sync, &config_data.source_dir, &config_data.root_dir, &config_data.installbin_dir, &config_data.custom_repo, &config_data.common_flags, &config_data.jobs};
    
    bool ERROR = false; 

    std::cout << GREEN << "|==========VAR-RESULTS=========|" << NC << std::endl;

    for(int i = 0; i < 7; i++){
      conf.lookupValue(vars_name[i], *vars[i]);
      if(vars_name[i] == "jobs" && vars[i]->empty() || vars_name[i] == "common_flags" && vars[i]->empty()){
        std::cerr << vars_name[i] << "...   [ " << YELLOW << "WARNING" << NC << " ]" << std::endl;
      } else if(vars[i]->empty()){
        ERROR = true;
        std::cerr << vars_name[i] << "...   [ " << RED << "FAILED" << NC << " ]" << std::endl;
      } else {
        std::cerr << vars_name[i] << "...   [ " << GREEN << "OK"<< NC << " ]" << std::endl;
      }
    }

      std::cout << GREEN << "|==============================|" << NC << std::endl;

    if(ERROR){
      throw std::runtime_error("Error while trying to load some var");
    }
    
    return EXIT_SUCCESS;
  }

  catch(const std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch(const libconfig::FileIOException &fioex){
    std::cerr << RED << "ERROR: " << NC << "I/O error reading build file ->" << RED << fioex.what() << NC << std::endl;
    return EXIT_FAILURE;
  }

  catch(const libconfig::ParseException &paex){
    std::cerr << RED << "ERROR: " << NC <<  "Parse error in build file -> " << GREEN << paex.getFile() << NC <<" -> " << RED << paex.getError() << NC << " ->" << " line -> " << RED << paex.getLine() << NC << std::endl;
    return EXIT_FAILURE; 
  }
}

int verify_dirs(const std::string* dirs[], const int num_dirs, const std::string& warning_this){
  for(int i = 0; i < num_dirs; i++){
    if(!check_dir(*dirs[i])){
      if(*dirs[i] == warning_this){
        std::cerr << YELLOW << "WARNING: " << NC << "repository directory -> " << GREEN << *dirs[i] << NC << " does not exist, use" << GREEN << " lalapkg --sync" << NC << std::endl;  
      } else {
        std::cerr << YELLOW << "WARNING: " << NC << "Directory -> " << GREEN << *dirs[i] << NC << " not found" << std::endl;
        std::cout << ">>> Creating Directory -> " << GREEN << *dirs[i] << "..." << NC << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(350));
        
        system(("mkdir -p " + *dirs[i]).c_str());
      }
    }
  }
  return EXIT_SUCCESS;
}
//==========================================================| MAIN
int main(int argc, char* argv[]){
  const std::string* dirs[] = {&repo_dir, &world_dir, &config_data.source_dir, &config_data.root_dir, &config_data.installbin_dir};
  const size_t num_elements = sizeof(dirs) / sizeof(dirs[0]);  
  const std::string user = getenv("USER");

  if(user != "root"){
    std::cerr << RED << ">>> ERROR: " << NC << "Are u sudo?" << std::endl;
    return EXIT_FAILURE;
  }
  
  if(load_config_file(config_file) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(!verify_dirs(dirs, num_elements, repo_dir)){
    return EXIT_FAILURE;
  }

  std::cout << *dirs[1] << std::endl;
  
  if(check_dir("/var/lalapkg/world/")){
    std::cout << "Existe" << std::endl;
  } else {
    std::cout << "Não existe" << std::endl;
  }
  return EXIT_SUCCESS;
}
