//==========================================|
//   FILE: package.h                        |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

class Package{
public:
  std::string pkgname;
  std::string pkgversion;
  std::string pkgsource;
  std::string pkgdesc;
  std::string pkgextension;
  std::string pkginfo_locale;
  std::string pkgscript_locale;
public:
  Package(const std::string& pkginfo_locale, const std::string& pkgscript_locale);

  
};

#endif
