//==========================================|
//   FILE: package.cc                       |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <filesystem>

#include "package.h"
#include "check.h"
#include "package_operations.h"
#include "animation.h"
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

std::string Package::get_pkgname() const{
  return this->pkgname;
}

std::string Package::get_pkgversion() const{
  return this->pkgversion;
}

void Package::run_vector_functions(std::vector<std::string>& vector_functions, std::string& source_dir){
  for(const auto& functions : vector_functions){
    int result = system(("cd " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + " && source " + this->pkgscript_locale + " && " + functions).c_str());
    if(result != 0){
      std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
      std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
      throw std::runtime_error("Error in function -> " GREEN + functions + NC ", file -> " GREEN + this->pkgscript_locale + NC);
    }
  }
}

int Package::makepkg(std::string& source_dir){
  std::atomic<bool> done = false;

  if(!check_is_file(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension)) { 

    std::thread animationThread(animateLoading, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Installing source: " GREEN + this->pkgsource + NC);

    int result = system(("wget -O " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + " " + this->pkgsource + " &> /dev/null").c_str());

    done = true;

    animationThread.join();

    if(result != 0){
      std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
      throw std::runtime_error("Unable to download package source -> " GREEN + this->pkgsource + NC);
    }
  } else {
    std::cout << YELLOW << ">>> " << NC << "Source is already installed! Skipping..." << std::endl; 
  }

  done = false;

  std::thread animationThread(animateLoading, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Unpacking: " GREEN + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + NC);

  int result = system(("cd " + source_dir + " && tar xpvf " + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + " &> /dev/null").c_str());

  done = true;

  animationThread.join();

  if(result != 0){
    std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
    std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
    throw std::runtime_error("Error unpacking tarball: " RED + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + NC);
  }
  
  Package::run_vector_functions(build_functions, source_dir);

  return EXIT_SUCCESS;
}

int Package::installpkg(const std::string& world_dir, std::string& source_dir, std::string& pkgs_dir, std::string& root_dir){
   
  const char* fakeroot_var_name = "FAKEROOT";
  std::string fakeroot_value = getenv("FAKEROOT");
  fakeroot_value += "/" + this->pkgname + "-" + this->pkgversion;
  
  setenv(fakeroot_var_name,(fakeroot_value).c_str(), 1);

  Package::run_vector_functions(install_functions, source_dir);

  const std::string pkgs = pkgs_dir + "/" + this->pkgname + "-" + this->pkgversion + ".lala.tar.gz";

  if(!check_is_file(pkgs)){
    std::atomic<bool> done = false;

    std::thread animationThread(animateLoading, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Creating package: " GREEN + this->pkgname + "-" + this->pkgversion + NC);

    int result = system(("cd $FAKEROOT && tar cvzf " + pkgs + " . &> /dev/null").c_str());

    done = true;

    animationThread.join();

    if(result != 0){
      throw std::runtime_error("Error creating package");
    } else {
      std::cerr << GREEN << ">>> " << NC <<"Package created in: " << GREEN << pkgs << NC << std::endl;
    }
  } else {
    std::cerr << YELLOW << ">>> " << NC << "Package already exists! Skipping..." << std::endl;
  }

  if(!check_is_dir(root_dir + "/" + world_dir + "/" + this->pkgname)){
    std::filesystem::create_directories(root_dir + "/" + world_dir + "/" + this->pkgname);
  }

  system(("tar xpvf " + pkgs + " -C " + root_dir + " > " + root_dir + "/" + world_dir + this->pkgname + "/world").c_str());

  std::filesystem::copy_file(this->pkginfo_locale, root_dir + "/" + world_dir + this->pkgname + "/info", std::filesystem::copy_options::overwrite_existing);

  std::filesystem::remove_all(fakeroot_value);

  return 0;
}

