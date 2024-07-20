//==========================================|
//   FILE: locker.h                         |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef LOCK_H
#define LOCK_H

#include <string>

class Locker{

private:
  
  static const std::string lockDir;
  static const std::string lockfileName;
  
  static bool locked;

public:
  
  static std::string getFile();

  static bool is_Locked();

  static int lock();
  static int unlock();
  
  static void waiting_Unlock();

};

#endif
