//=========================================|
//   FILE: package_operations.cc           |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#include <cstdlib>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <stdexcept>
#include <libconfig.h++>

#include "package_operations.h"
#include "check.h"
#include "color.h"

std::string package_exist(const std::string& repo, std::string& package_name, const std::string& script_file_name, const std::string& info_file_name){
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
          if(check_is_file(dirs + "/" + package_name + "/" + info_file_name)){
            return dirs + "/" + package_name + "/";
          } else {
            throw std::runtime_error("Info file not found in -> " GREEN + dirs + "/" + package_name + "/" + info_file_name + NC);
          }
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

int loadenv_var(std::string& common_flags, std::string& jobs, const std::string& installbin_dir, std::string& pkg_dir){ 
  const char* var_name[] = {"CFLAGS", "CXXFLAGS", "MAKEOPTS", "FAKEROOT", "PKGS"};
  const char* var_value[] = {common_flags.c_str(), common_flags.c_str(), jobs.c_str(), installbin_dir.c_str(), pkg_dir.c_str()};

  const size_t size = sizeof(var_name) / sizeof(var_name[0]);

  for(int i = 0; i < size; i++){
    setenv(var_name[i], var_value[i], 1);
    char* check_var = getenv(var_name[i]);
    if(check_var == nullptr){
      std::cerr << RED << "ERROR: " << NC << "Unable to set variable -> " RED << var_name[i] << NC << std::endl;  
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void unsetenv_var(){
  const char* var_name[] = {"CFLAGS", "CXXFLAGS", "MAKEOPTS", "FAKEROOT", "PKGS"};

  for(int i = 0; i < sizeof(var_name) / sizeof(var_name[0]); i++){
    if(unsetenv(var_name[i]) != 0){
      std::cerr << YELLOW << "WARNING: " << NC << "Unable to unset variable -> " << var_name[i] << std::endl;
    }
  }
}

int get_infos(std::string* var_ptr[], std::string& pkginfo_locale){
  libconfig::Config file;

  const std::string lookup_vars[] = {"PKGNAME", "PKGVERSION", "PKGSOURCE", "PKGDESC", "PKGEXTENSION"};
  
  try{
    file.readFile(pkginfo_locale);

    for(int i = 0; i < sizeof(lookup_vars) / sizeof(lookup_vars[0]); i++){
      file.lookupValue(lookup_vars[i], *var_ptr[i]);
    }

    for(int i = 0; i < sizeof(lookup_vars) / sizeof(lookup_vars[0]); i++){
      if(*var_ptr[i] == ""){
        throw std::runtime_error("Error while try to load var -> " GREEN + lookup_vars[i] + NC " Check info file -> " GREEN + pkginfo_locale + NC);
      }
    }
    return EXIT_SUCCESS;
  }

  catch(libconfig::FileIOException &fioex){
    std::cerr << "ERROR: " << NC << "I/O error reading info file -> " << RED << fioex.what() << NC << std::endl; 
    return EXIT_FAILURE;
  }

  catch(libconfig::ParseException &paex){
    std::cerr << "ERROR: " << NC << "Parser error in info file -> " << GREEN << paex.getFile() << NC << " -> " << RED << paex.getError() << NC << " -> " << "line -> " << RED << paex.getLine() << NC << std::endl;
    return EXIT_FAILURE;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }
}

int get_functions(std::vector<std::string>& build_functions, std::vector<std::string>& install_functions, std::string& pkgscript_locale){
  std::string possible_build_functions[] = {"pre_build", "build", "pos_build"};
  std::string possible_install_functions[] = {"pre_install", "install", "pos_install"};

  int result;

  for(int i = 0; i < sizeof(possible_build_functions) / sizeof(possible_build_functions[0]); i++){
    result = system(("source " + pkgscript_locale + " && declare -f " + possible_build_functions[i] + " &> /dev/null").c_str());
    
    if(result == 0){
      build_functions.push_back("source " + pkgscript_locale + " && " + possible_build_functions[i]);
    }
  
    result = system(("source " + pkgscript_locale + " && declare -f " + possible_install_functions[i] + " &> /dev/null").c_str());

    if(result == 0){
      install_functions.push_back("source " + pkgscript_locale + " && " + possible_install_functions[i]);
    }
  }

  try{
    if(build_functions.empty()){
      throw std::runtime_error(YELLOW "Build functions " NC "not found in -> "  GREEN + pkgscript_locale + NC);
    }

    if(install_functions.empty()){
      throw std::runtime_error(YELLOW "Install functions " NC "not found in -> " GREEN + pkgscript_locale + NC);
    }
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
