//=========================================|
//   FILE: package_operations.h            |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#ifndef PACKAGE_OPERATIONS_H
#define PACKAGE_OPERATIONS_H

#include <string>

std::string package_exist(const std::string& repo, std::string& package_name, const std::string& script_file_name);

#endif 
