//==========================================|
//   FILE: signalHandler.cc                 |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <csignal>
#include <iostream>
#include <filesystem>

#include "signalHandler.h"

std::string del_file; 

void signalHandler(int signal){

  if(SIGINT == signal){

    std::filesystem::remove(del_file);

  }

  std::cout << std::endl;

  exit(signal);

}
