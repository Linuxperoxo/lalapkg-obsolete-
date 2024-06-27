//==========================================|
//   FILE: package.cc                       |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>

#include "package.h"
#include "check.h"
#include "package_operations.h"
#include "color.h"

Package::Package(const std::string& pkginfo_locale, const std::string& pkgscript_locale) : pkginfo_locale(pkginfo_locale), pkgscript_locale(pkgscript_locale){
  std::string* var_pts[] = {&this->pkgname, &this->pkgversion, &this->pkgsource, &this->pkgdesc, &this->pkgextension};

  if(get_infos(var_pts, this->pkginfo_locale) == EXIT_FAILURE){
    throw std::runtime_error("");
  }

  if(get_functions(this->build_functions, this->install_functions, this->pkgscript_locale)){
    throw std::runtime_error("");
  }
}

Package::~Package(){
  
}

void Package::run_vector_functions(std::vector<std::string>& vector_functions, std::string& source_dir){
  for(const auto& functions : build_functions){
    int result = system(("cd " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + " && source " + this->pkgscript_locale + " && " + functions).c_str());
    if(result != 0){
      std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
      std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
      throw std::runtime_error("Error in function -> " GREEN + functions + NC ", file -> " GREEN + this->pkgscript_locale + NC);
    }
  }
}

int Package::makepkg(std::string& source_dir, std::string& common_flags, std::string& jobs){
  const std::string* load_var[] = {&common_flags, &common_flags, &jobs};
  const std::string name_var[] = {"CFLAGS", "CXXFLAGS" ,"MAKEOPTS"};
  static bool is_load = false; 
  
   if(!is_load){
     for(int i = 0; i < sizeof(name_var) / sizeof(name_var[0]); i++){
       if(setenv(name_var[i].c_str(), load_var[i]->c_str(), 1) != 0){
         throw std::runtime_error("Error set env var -> "  GREEN + name_var[i] + NC);
       }
    }
    is_load = true;  
  }
    
  if(!check_is_file(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension)) { 
    std::cout << GREEN << ">>> " << NC << "Starting source download: " << GREEN << this->pkgsource << NC << std::endl;

    int result = system(("wget -O " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + " " + this->pkgsource).c_str());

    if(result != 0){
      throw std::runtime_error("Unable to download package source -> " GREEN + this->pkgsource + NC);
    }
  } else {
    std::cout << GREEN << ">>> " << NC << "Source is already installed! Skipping..." << std::endl; 
  }

  std::cout << GREEN << ">>> " << NC << "Unpacking: " << GREEN << this->pkgname + "-" + this->pkgversion + "." + this->pkgextension << NC << std::endl;

  int result = system(("cd " + source_dir + " && tar xpvf " + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension).c_str());

  if(result != 0){
    std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
    std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
    throw std::runtime_error("Error unpacking tarball: " RED + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + NC);
  }
  
  Package::run_vector_functions(build_functions, source_dir);

  std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
  return EXIT_SUCCESS;
}

int Package::installpkg(std::string& installbin_dir, const std::string& world_dir, std::string& source_dir){
  Package::run_vector_functions(install_functions, source_dir);
}

