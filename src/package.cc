//==========================================|
//   FILE: package.cc                       |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <string>

#include "package.h"
#include "package_operations.h"

Package::Package(const std::string& pkginfo_locale, const std::string& pkgscript_locale){
  this->pkginfo_locale = pkginfo_locale;
  this->pkgscript_locale = pkgscript_locale;
}
