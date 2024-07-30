//==========================================|
//   FILE: main.cc                          |
//   VERSION: 1.1.5                         |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

//==========================================================| LIBS

#include <cctype>
#include <csignal>
#include <cstddef>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <libconfig.h++>
#include <filesystem>

//==========================================================| MY HEADERS

#include "check.h"
#include "package_operations.h"
#include "package.h"
#include "color.h"
#include "config_file.h"
#include "locker.h"
#include "repo.h"
#include "signalHandler.h"

//==========================================================| CONST VARS

const std::string config_file = "/etc/lala.conf";
const std::string repo_dir = "/var/lalapkg/repo";
const std::string info_file = "Infopkg";
const std::string build_file = "Buildpkg";
const std::string world_dir = "/var/lalapkg/world/";
const std::string installbin_dir = "/tmp/lalapkg/fakeroot";

//==========================================================| PACKAGE VECTOR

std::vector<std::string> packages_vector;

Config_file* conf_file = new Config_file;

//==========================================================| FUNCTIONS

int emergepkg(std::string pkg){
  
  Package* newpkg = nullptr;

  try{
    
    const std::string pkgroot = package_exist(repo_dir, pkg, build_file, info_file);

    newpkg = new Package(pkgroot + info_file, pkgroot + build_file);

    const std::string pkgname = newpkg->get_pkgname();
    const std::string pkgversion = newpkg->get_pkgversion();

    newpkg->makepkg(conf_file->source_dir);
    newpkg->installpkg(world_dir, conf_file->source_dir, conf_file->pkg_dir, conf_file->root_dir);

    std::filesystem::remove_all(conf_file->source_dir + "/" + pkgname + "-" + pkgversion);

    delete newpkg;

  }

  catch(std::runtime_error &error){
    
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    
    std::cerr << RED << "ERROR: " << pkg << NC << " package failure!" << std::endl;

    Locker::unlock();
    
    delete newpkg;
    
    return EXIT_FAILURE;

  }
  
  return EXIT_SUCCESS;

}

int pkginfos(std::string pkg, char& info_arg){
  
  Package* ptr_pkg = nullptr;
  
  try{

    const std::string pkgroot = package_exist(repo_dir, pkg, build_file, info_file);

    ptr_pkg = new Package(pkgroot + info_file, pkgroot + build_file);

    ptr_pkg->view_pkginfos(info_arg);

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

      Locker::unlock();

    break;

    case 'u':

      for(const auto& vector : packages_vector){

        // unmerge
        
      }

    break;

    case 's':

      Locker::lock();

      if(Repo::sync() == EXIT_FAILURE){

        Locker::unlock();
        
        return EXIT_FAILURE;

      }

      Locker::unlock();

    break;

    case 'i':

      pkginfos(packages_vector[0], user_arg[1]);

    break;

  }

  return EXIT_FAILURE;

}

//==========================================================| MAIN

int main(int argc, char* argv[]){
  
  const std::string* check_this_dirs[] = {&conf_file->source_dir, &conf_file->root_dir, &conf_file->pkg_dir, &installbin_dir, &repo_dir, &world_dir}; 
  const size_t size = sizeof(check_this_dirs) / sizeof(check_this_dirs[0]);
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
  
  if(loadenv_var(conf_file->common_flags, conf_file->jobs, installbin_dir) == EXIT_FAILURE){
    
    return EXIT_FAILURE;

  }
  
  if(check_dirs(check_this_dirs, repo_dir, size) == EXIT_FAILURE){
    
    return EXIT_FAILURE;
    
  }

  if(check_argument(argv, argc, arg, packages_vector) == EXIT_FAILURE){
    
    return EXIT_FAILURE;
    
  }

  if(main_switch_loop(arg) == EXIT_FAILURE){
    
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;

}

