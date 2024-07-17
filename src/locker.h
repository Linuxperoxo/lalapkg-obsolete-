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
  
  const std::string lockDir = "/var/lalapkg/locker/";
  const std::string lockfileName = "lockfile";
  
  bool locked;

public:
  
  Locker();

  std::string getFile() const;

  bool is_Locked() const;

  int lock();
  int unlock();
  
  void waiting_Unlock();

};

#endif
