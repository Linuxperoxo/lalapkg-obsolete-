//==========================================|
//   FILE: signalHandler.cc                 |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <csignal>
#include <iostream>
#include <filesystem>

#include "signalHandler.h"

std::string SignalHandler::del_file;

void SignalHandler::signalHandler(int signal){

  std::filesystem::remove(del_file);
  
  std::cout << std::endl;

  exit(signal);

}

void SignalHandler::exitSignal(const std::string& file){
  
  del_file = file;

  signal(SIGINT, SignalHandler::signalHandler);

}
