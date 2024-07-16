#ifndef LOCK_H
#define LOCK_H

#include <string>

class Locker{

private:
  
  bool Locked;

  const std::string lockdir = "/var/lalapkg/locker/";
  
  const std::string lockfile_name = "lockfile";

public:
  
  Locker();

  bool is_Locked() const;

  int lock_Process();

  int unlock();

  void waiting_Unlock();

};

#endif
