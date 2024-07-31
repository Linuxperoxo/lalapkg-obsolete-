//=========================================|
//   FILE: check.h                         |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/

#ifndef CHECK_H
#define CHECK_H

//==========================================================| LIBS

#include <string>
#include <vector>

//==========================================================| DECLARING FUNCTIONS

bool check_is_file(const std::string& file);
bool check_is_dir(const std::string& dir);
bool checkCommand(const std::string command);

int check_dirs(const std::vector<const std::string*> dirs);

int check_argument(char* arg[], int& num_args, char user_arg[], std::vector<std::string>& packages_vector);

#endif
