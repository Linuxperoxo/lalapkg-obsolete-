//==========================================|
//   FILE: main.cc             ''             |
//   VERSION: -                             |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

//==========================================================| LIBS

#include <cctype>
#include <csignal>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <libconfig.h++>
#include <filesystem>

//==========================================================| MY HEADERS

#include "check.h"
#include "package.h"
#include "color.h"
#include "config_file.h"
#include "locker.h"
#include "env_var.h"
#include "repo.h"
#include "signalHandler.h"

//==========================================================| CONST VARS

const std::string config_file = "/etc/lala.conf";
const std::string repo_dir = "/var/lalapkg/repo";
const std::string world_dir = "/var/lalapkg/world/";
const std::string installbin_dir = "/tmp/lalapkg/fakeroot";

//==========================================================| PACKAGE VECTOR

std::vector<std::string> packages_vector;

Config_file* conf_file = new Config_file;

//==========================================================| FUNCTIONS

int emergepkg(std::string pkg){
  Package* newpkg = nullptr;

  try{
    Package::packageExist(repo_dir, pkg);

    newpkg = new Package();
    newpkg->makepkg(conf_file->source_dir);
    newpkg->installpkg(world_dir, conf_file->source_dir, conf_file->pkg_dir, conf_file->root_dir);

    std::filesystem::remove_all(conf_file->source_dir + "/" + newpkg->getPkgname() + "-" + newpkg->getPkgversion());

    delete newpkg;

    return EXIT_SUCCESS;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    
    std::cerr << RED << "ERROR: " << pkg << NC << " package failure!" << std::endl;
  }

  catch(libconfig::FileIOException& fioex){
    std::cerr << RED "ERROR: " NC << "I/O error reading info file -> " RED << newpkg->getPkgInfoFile() << ' ' << fioex.what() << NC << '\n';
  }

  catch(libconfig::ParseException& paex){
    std::cerr << RED "ERROR: " NC << "Parser error in info file ->  " RED << paex.getFile() << NC "! ERROR -> " RED << paex.what() << ' ' << paex.getError() << NC << '\n';
  }

  delete newpkg;
  return EXIT_FAILURE;
}

int pkginfos(std::string pkg, char& info_arg){
  Package* ptr_pkg = nullptr;
  
  try{
    Package::packageExist(repo_dir, pkg);

    ptr_pkg = new Package();

    ptr_pkg->viewPkginfos(info_arg);

    delete ptr_pkg;

    return EXIT_SUCCESS;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    
    Locker::unlock();

    delete ptr_pkg;

    return EXIT_FAILURE;
  }
}

int main_switch_loop(char user_arg[]){
  switch(user_arg[0]){
    case 'e':
      Locker::lock();

      for(const auto& vector : packages_vector){
        emergepkg(vector);
      }
      return EXIT_SUCCESS;
    break;

    case 'u':
      for(const auto& vector : packages_vector){
        // unmerge
      }
    break;

    case 's':
      Locker::lock();

      if(Repo::sync() == EXIT_FAILURE){
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    break;

    case 'i':
      pkginfos(packages_vector[0], user_arg[1]);
      return EXIT_SUCCESS;
    break;
  }
}

//==========================================================| MAIN

int main(int argc, char* argv[]){  
  const std::vector<const std::string*> check_this_dirs = {&conf_file->source_dir, &conf_file->root_dir, &conf_file->pkg_dir, &installbin_dir, &world_dir}; 
  const std::string user_name = getenv("USER");
  const std::string file = Locker::getFile();
  
  char arg[3];

  if(user_name != "root"){
    std::cerr << RED << "ERROR: " << NC << "Are u sudo?" << std::endl;
    return EXIT_FAILURE;
  }
  
  if(Locker::is_Locked()){
    Locker::waiting_Unlock();
  }

  SignalHandler::exitSignal(file);

  if(load_config(config_file, conf_file) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  Env* env = new Env(conf_file->common_flags, conf_file->common_flags, conf_file->jobs, installbin_dir);
  
  if(env->_loadenv_() == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(check_dirs(check_this_dirs) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(check_argument(argv, argc, arg, packages_vector) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(main_switch_loop(arg) == EXIT_FAILURE){
    delete env;
    delete conf_file;
    Locker::unlock();
    return EXIT_FAILURE;
  }

  delete env;
  delete conf_file;
  Locker::unlock();

  return EXIT_SUCCESS;
}

