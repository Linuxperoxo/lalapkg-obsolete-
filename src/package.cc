//==========================================|
//   FILE: package.cc                       |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <atomic>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <filesystem>
#include <libconfig.h++>

#include "package.h"
#include "check.h"
#include "animation.h"
#include "color.h"

const std::string Package::build_file = "Buildpkg";
const std::string Package::info_file = "Infopkg";

std::string Package::pkginfo_locale;
std::string Package::pkgscript_locale;

Package::Package() : compiled(false) {
  if(getInfos() == EXIT_FAILURE){
    throw std::runtime_error("");
  }

  if(Package::getFunctions(this->build_functions, this->install_functions)){
    throw std::runtime_error("");
  }
}

std::string Package::getPkgInfoFile() const{
  return this->pkginfo_locale;
}

std::string Package::getPkgname() const{
  return this->pkgname;
}

std::string Package::getPkgversion() const{
  return this->pkgversion;
}

int Package::getInfos(){
  libconfig::Config file;

  const std::vector<std::string> lookup_vars = {"PKGNAME", "PKGVERSION", "PKGSOURCE", "PKGDESC", "PKGEXTENSION"};
  const std::vector<std::string*> vars_pts = {&this->pkgname, &this->pkgversion, &this->pkgsource, &this->pkgdesc, &this->pkgextension};

  
  file.readFile(pkginfo_locale);

  for(int i = 0; i < lookup_vars.size(); i++){
    file.lookupValue(lookup_vars[i], *vars_pts[i]);
  }

  unsigned int index = 0;

  for(const auto& vars : vars_pts){
    if(*vars == ""){
      throw std::runtime_error("Error while try to load var -> " RED + lookup_vars[index] + NC "! Check info file -> " RED + pkginfo_locale + NC);
    }
    index++;
  }
  return EXIT_SUCCESS;
}

void Package::packageExist(const std::string& repo, const std::string& package_name){
  std::vector<std::string> repo_sub_dirs;

  if(!check_is_dir(repo)){
    throw std::runtime_error("Repository directory -> " RED + repo + NC " does not exist, use " RED "lalapkg -s" NC);
  }

  for(const auto& sub_dirs : std::filesystem::directory_iterator(repo)){
    if(std::filesystem::is_directory(sub_dirs)){
      std::string dir = sub_dirs.path().string();
      repo_sub_dirs.push_back(dir);
    }
  }

  bool found = false;

  for(const auto& dirs : repo_sub_dirs){
    if(check_is_dir(dirs + "/" + package_name)){
      found = true;

      if(!check_is_file(dirs + "/" + package_name + "/" + build_file) || !check_is_file(dirs + "/" + package_name + "/" + info_file)){
        throw std::runtime_error(RED + info_file + NC " or " + RED + build_file + NC " not found in -> " RED + dirs + "/" + package_name + NC);
      }

      pkginfo_locale = dirs + "/" + package_name + "/" + info_file;
      pkgscript_locale = dirs + "/" + package_name + "/" + build_file;
    }
  }

  if(!found){
    throw std::runtime_error("Package -> " RED + package_name + NC " not found");
  }
}

void Package::viewPkginfos(char info){
  switch(tolower(info)){
    case 'a':
      std::cout << GREEN << "PKG_VERSION: " << NC << this->pkgversion << std::endl;
      std::cout << GREEN << "PKG_SOURCE: " << NC << this->pkgsource << std::endl;
      return;
    break;

    case 'v':
      std::cout << GREEN << "PKG_VERSION: " << NC << this->pkgversion << std::endl;
      return;
    break;

    case 's':
      std::cout << GREEN << "PKG_SOURCE: " << NC << this->pkgsource << std::endl;
    break;

    default:
      throw std::runtime_error("Invalid option! Use ia(all infos), iv(pkg version), is(pkg source)");
    break;
  }
}

int Package::getFunctions(std::vector<std::string>& build_functions, std::vector<std::string>& install_functions){
  const std::vector<std::string> possible_build_functions = {"pre_build", "build", "pos_build"};
  const std::vector<std::string> possible_install_functions = {"pre_install", "install", "pos_install"};

  for(int i {0}; i < possible_build_functions.size(); i++){
    if(system(("source " + this->pkgscript_locale + " && declare -f " + possible_build_functions[i] + " &> /dev/null").c_str()) == 0){
      build_functions.push_back("source " + this->pkgscript_locale + " && " + possible_build_functions[i]);
    }

    if(system(("source " + this->pkgscript_locale + " && declare -f " + possible_install_functions[i] + " &> /dev/null").c_str()) == 0){
      install_functions.push_back("source " + pkgscript_locale + " && " + possible_install_functions[i]);
    }
  }

  if(install_functions.empty()){
    throw std::runtime_error(YELLOW "Install functions " NC "not found in -> " RED + pkgscript_locale + NC);
  }
  return EXIT_SUCCESS;
}

void Package::runVectorFunctions(const std::vector<std::string>& vector_functions, const std::string& source_dir){
  for(const auto& functions : vector_functions){
    
    if(system(("cd " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + " && source " + this->pkgscript_locale + " && " + functions).c_str()) != 0){ 
      std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
      std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);

      throw std::runtime_error("Error in function -> " GREEN + functions + NC);
    }
  }
}

int Package::makepkg(const std::string& source_dir){
  if(!check_is_file(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension)){ 
    std::atomic<bool> done = false;
    
    std::thread animationDownload(animate, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Installing source: " GREEN + this->pkgsource + NC, 'z');

    int result = system(("wget -O " + source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + " " + this->pkgsource + " &> /dev/null").c_str());

    done.store(true);

    animationDownload.join();

    if(result != 0){
      std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
      throw std::runtime_error("Unable to download package source -> " GREEN + this->pkgsource + NC);
    }
  } else { 
    std::cout << YELLOW << ">>> " << NC << "Source is already installed! Skipping..." << std::endl; 
  }

  std::atomic<bool> done = false;

  std::thread animationUnpacking(animate, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Unpacking: " GREEN + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + NC, 'z');

  int result = system(("cd " + source_dir + " && tar xpvf " + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + " &> /dev/null").c_str());

  done.store(true);

  animationUnpacking.join();

  if(result != 0){
    std::filesystem::remove(source_dir + "/" + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension);
    std::filesystem::remove_all(source_dir + "/" + this->pkgname + "-" + this->pkgversion);
    throw std::runtime_error("Error unpacking tarball: " RED + this->pkgname + "-" + this->pkgversion + "." + this->pkgextension + NC);
  }
  
  if(!build_functions.empty()){
    std::cout << BLUE "[" GREEN "***" BLUE "] " NC "Compiling: " GREEN << this->pkgname << "-" << this->pkgversion << NC << '\n';

    Package::runVectorFunctions(build_functions, source_dir);

    this->compiled = true;
  }
  return EXIT_SUCCESS;
}

int Package::installpkg(const std::string& world_dir, const std::string& source_dir, const std::string& pkgs_dir, const std::string& root_dir){
  const char* fakeroot_var_name = "FAKEROOT";

  std::string fakeroot_value = getenv("FAKEROOT");
  
  fakeroot_value += "/" + this->pkgname + "-" + this->pkgversion;
  
  setenv(fakeroot_var_name,(fakeroot_value).c_str(), 1);

  Package::runVectorFunctions(install_functions, source_dir);

  const std::string pkgs = pkgs_dir + "/" + this->pkgname + "-" + this->pkgversion + ".lala.tar.gz";

  if(this->compiled || !check_is_file(pkgs)){
    std::atomic<bool> done = false;

    std::thread animationThread(animate, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Creating package: " GREEN + this->pkgname + "-" + this->pkgversion + NC, 'z');

    int result = system(("cd $FAKEROOT && tar cvzf " + pkgs + " . &> /dev/null").c_str());

    done.store(true);

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

  std::atomic<bool> done = false;

  std::thread animationThread(animate, std::ref(done), BLUE "[" GREEN "***" BLUE "] " NC "Installing " GREEN + this->pkgname + "-" + this->pkgversion + NC " in "  GREEN + root_dir + NC, 'i');
  
  system(("tar xpvf " + pkgs + " -C " + root_dir + " > " + root_dir + "/" + world_dir + this->pkgname + "/world").c_str());

  done.store(true);

  animationThread.join();

  std::filesystem::copy_file(this->pkginfo_locale, root_dir + "/" + world_dir + this->pkgname + "/info", std::filesystem::copy_options::overwrite_existing);
  std::filesystem::remove_all(fakeroot_value);

  return EXIT_SUCCESS;
}

