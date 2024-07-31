//=========================================|
//   FILE: package_operations.cc           |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <libconfig.h++>

#include "package_operations.h"
#include "color.h"

int loadenv_var(std::string& common_flags, std::string& jobs, const std::string& installbin_dir){ 
  const char* var_name[] = {"CFLAGS", "CXXFLAGS", "MAKEFLAGS", "FAKEROOT"};
  const char* var_value[] = {common_flags.c_str(), common_flags.c_str(), jobs.c_str(), installbin_dir.c_str()};

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
  const char* var_name[] = {"CFLAGS", "CXXFLAGS", "MAKEFLAGS", "FAKEROOT"};

  for(int i = 0; i < sizeof(var_name) / sizeof(var_name[0]); i++){
    if(unsetenv(var_name[i]) != 0){
      std::cerr << YELLOW << "WARNING: " << NC << "Unable to unset variable -> " << var_name[i] << std::endl;
    }
  }
}

int get_infos(std::string* var_ptr[], std::string& pkginfo_locale){
  libconfig::Config file;

  const std::vector<std::string> lookup_vars = {"PKGNAME", "PKGVERSION", "PKGSOURCE", "PKGDESC", "PKGEXTENSION"};
  
  try{
    file.readFile(pkginfo_locale);

    for(int i = 0; i < lookup_vars.size(); i++){
      file.lookupValue(lookup_vars[i], *var_ptr[i]);
    }

    for(int i = 0; i < lookup_vars.size(); i++){
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

