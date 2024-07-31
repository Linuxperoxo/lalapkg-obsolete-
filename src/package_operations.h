//=========================================|
//   FILE: package_operations.h            |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#ifndef PACKAGE_OPERATIONS_H
#define PACKAGE_OPERATIONS_H

#include <string>
#include <vector>

int get_infos(std::string* vars_ptr[], std::string& pkginfo_locale);

int loadenv_var(std::string& common_flags, std::string& jobs, const std::string& installbin_dir);

void unsetenv_var();

#endif 
