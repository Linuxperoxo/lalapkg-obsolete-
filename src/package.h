//==========================================|
//   FILE: package.h                        |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

#include "color.h"
#include "check.h"

class Package{
private:

  //==================================| Pkg vars
  
  std::string pkgname;
  std::string pkgversion;
  std::string pkgsource;
  std::string pkgdesc;
  std::string pkgextension;
  
  static std::string pkginfo_locale;
  static std::string pkgscript_locale;

  static const std::string build_file;
  static const std::string info_file;

  //==================================| Functions vector

  std::vector<std::string> build_functions;
  std::vector<std::string> install_functions;

public:

  Package();
  
  ~Package();

  static void package_exist(const std::string& repo, std::string& package_name);

  int makepkg(std::string& source_dir);
  int installpkg(const std::string& world_dir, std::string& source_dir, std::string& pkgs_dir, std::string& root_dir);
  
  void run_vector_functions(std::vector<std::string>& vector_functions, std::string& source_dir);
  void view_pkginfos(char info);

  std::string get_pkgname() const;
  std::string get_pkgversion() const;
};

#endif
