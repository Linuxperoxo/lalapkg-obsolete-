//=========================================|
//   FILE: env_var.cc                      |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

//==========================================================| LIBS

#include <cstdlib>
#include <iostream>
#include <vector>

//==========================================================| .H

#include "env_var.h"
#include "color.h"

//==========================================================| DECLARING CLASS FUNCTIONS

Env::~Env(){
  const std::vector<std::string> env_names = {"CFLAGS", "CXXFLAGS", "JOBS", "FAKEROOT"};

  for(const auto& name : env_names){
    if(unsetenv(name.c_str()) != 0){
      std::cerr << YELLOW "WARNING: " NC "Unable to unset variable -> " YELLOW << name << NC << '\n';
    }
  }
}

int Env::_loadenv_(){
  const std::vector<std::string> env_names = {"CFLAGS", "CXXFLAGS", "JOBS", "FAKEROOT"};
  const std::vector<const std::string*> env_values = {&this->CFLAGS, &this->CXXFLAGS, &this->JOBS, &this->FAKEROOT};

  for (size_t i = 0; i < env_names.size(); ++i) {
    setenv(env_names[i].c_str(), env_values[i]->c_str(), 1);
    char* check_var = getenv(env_names[i].c_str());
    if(check_var == nullptr){
      std::cerr << RED << "ERROR: " << NC << "Unable to set variable -> " RED << env_names[i] << NC << std::endl;  
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
