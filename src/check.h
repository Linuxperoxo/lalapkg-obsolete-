//=========================================|
//   FILE: check.h                         |
//   AUTHOR: Linuxperoxo                   |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.  |
//=========================================/


#ifndef CHECK_H
#define CHECK_H

#include <string>
#include <vector>

bool check_is_file(const std::string& file);
bool check_is_dir(const std::string& dir);
bool checkCommand(const std::string command);

int check_dirs(const std::string* dirs[], const std::string& warning_dir, const int num_dirs);

int check_argument(char* arg[], int& num_args, char user_arg[], std::vector<std::string>& packages_vector);

#endif
