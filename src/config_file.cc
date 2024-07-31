//==========================================|
//   FILE: config_file.cc                   |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ostream>
#include <string>
#include <libconfig.h++>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "color.h"
#include "config_file.h"

int load_config(const std::string &file, Config_file* conf_file){
  libconfig::Config conf_obj;

  try{
    conf_obj.readFile(file);

    std::vector<std::string> var_warning;
    std::vector<std::string> var_failed;

    std::string var_names[5] = {"source_dir", "pkg_dir", "root_dir", "common_flags", "jobs"};
    std::string* var_pts[5] = {&conf_file->source_dir, &conf_file->pkg_dir, &conf_file->root_dir, &conf_file->common_flags, &conf_file->jobs};
    
    int i = 0;
    
    for(const auto& names : var_names){
      conf_obj.lookupValue(names, *var_pts[i]);
      if(var_pts[i]->empty()){
        if(names == "common_flags" || names == "jobs"){
          var_warning.push_back(names);
        } else {
          var_failed.push_back(names);
        }
      }
      i++;
    }

    if(!var_failed.empty()){
      std::cout << RED << "|==========VAR-FAILS=========|" << NC << std::endl;
      for(const auto& failed : var_failed){
        std::cerr << RED << "| " << NC << "[" << RED << "***" << NC << "] " << failed << NC << std::endl;  
      }
      std::cout << RED << "|============================|" << NC << std::endl;
      throw std::runtime_error("Error while trying to load some var: Check config file -> " RED + file + NC);
    }

    if(!var_warning.empty()){
      std::cout << YELLOW << "|==========VAR-WARNINGS=========|" << NC << std::endl;
      for(const auto& warning : var_warning){
        std::cerr << YELLOW << "| " << NC << "[" << YELLOW << "***" << NC << "] " << warning << std::endl;
      }
      std::cout << YELLOW << "|===============================|" << NC << std::endl;

      std::string user_buffer;

      std::cerr << YELLOW << "WARNING: " << NC << "Do you want to continue even without setting these variables? " << GREEN << "Y" << NC << "/" << RED << "n" << NC << std::endl;
      getline(std::cin, user_buffer);

      while(true){
        if(user_buffer[1] == '\0' || user_buffer[0] == '\0'){
          switch(std::tolower(user_buffer[0])){
            case 'y':
              return EXIT_SUCCESS;
            break;

            case 'n':
              return EXIT_FAILURE;
            break;

            case '\0':
              return EXIT_SUCCESS;
            break;

            case ' ':
              return EXIT_SUCCESS;
            break;
          }
        } else {
          std::cout << "I didn't understand! " << GREEN << "Y" << NC << "/" << RED << "n" << NC << std::endl;
          getline(std::cin, user_buffer);
        }
      }
    }
    return EXIT_SUCCESS;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch(libconfig::ParseException &paex){
    std::cerr << RED "ERROR: " NC "Parse error in config file -> " GREEN << paex.getFile() << NC " -> " RED << paex.getError() << NC " line -> " << RED << paex.getLine() << NC << std::endl;
    return EXIT_FAILURE;
  }

   catch(libconfig::FileIOException &fioex){
    std::cerr << RED "ERROR: " NC "I/O error reading config file -> " RED << file << NC << std::endl;
    return EXIT_FAILURE;
  }
}
