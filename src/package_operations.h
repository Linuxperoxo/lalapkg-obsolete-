//=========================================|
//   FILE: package_operations.h            |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#ifndef PACKAGE_OPERATIONS_H
#define PACKAGE_OPERATIONS_H

#include <string>
#include <vector>

std::string package_exist(const std::string& repo, std::string& package_name, const std::string& script_file_name, const std::string& info_file_name);
int get_infos(std::string* vars_ptr[], std::string& pkginfo_locale);
int get_functions(std::vector<std::string>& build_functions, std::vector<std::string>& install_functions, std::string& pkgscript_locale);

#endif 
