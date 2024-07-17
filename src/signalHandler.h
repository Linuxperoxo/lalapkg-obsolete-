//==========================================|
//   FILE: signalHandler.h                  |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <string>

class SignalHandler{

private:
  
  static std::string del_file;

  static void signalHandler(int signal);

public:

  static void exitSignal(const std::string& file);

};

#endif
