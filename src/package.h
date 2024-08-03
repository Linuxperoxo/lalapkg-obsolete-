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
  
  bool compiled;
  
  static std::string pkginfo_locale;
  static std::string pkgscript_locale;

  static const std::string build_file;
  static const std::string info_file;

  //==================================| Functions vector

  std::vector<std::string> build_functions;
  std::vector<std::string> install_functions;

  int getFunctions(std::vector<std::string>& build_functions, std::vector<std::string>& install_functions);
  int getInfos();

public:

  Package();
  
  ~Package() = default;

  static void packageExist(const std::string& repo, const std::string& package_name);

  int makepkg(const std::string& source_dir);
  int installpkg(const std::string& world_dir, const std::string& source_dir, const std::string& pkgs_dir, const std::string& root_dir);

  void runVectorFunctions(const std::vector<std::string>& vector_functions, const std::string& source_dir);
  void viewPkginfos(char info);

  std::string getPkgname() const;
  std::string getPkgversion() const;
  std::string getPkgInfoFile() const;
};

#endif
