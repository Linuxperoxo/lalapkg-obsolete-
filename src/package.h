//==========================================|
//   FILE: package.h                        |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <vector>

class Package{
private:

  //==================================| Pkg vars
  
  std::string pkgname;
  std::string pkgversion;
  std::string pkgsource;
  std::string pkgdesc;
  std::string pkgextension;
  std::string pkginfo_locale;
  std::string pkgscript_locale;

  //==================================| Functions vector

  std::vector<std::string> build_functions;
  std::vector<std::string> install_functions;

public:

  Package(const std::string& pkginfo_locale, const std::string& pkgscript_locale);
  ~Package();

  int makepkg(std::string& source_dir,std::string& common_flags, std::string& jobs);
  int installpkg();
};

#endif
