//==========================================|
//   FILE: env_var.h                        |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef ENV_VAR_H
#define ENV_VAR_H

//==========================================================| LIBS

#include <vector>
#include <string>

//==========================================================| DECLARING CLASS 

class Env{
private:
  const std::string CFLAGS;
  const std::string CXXFLAGS;
  const std::string JOBS;
  const std::string FAKEROOT;
public:
  Env(const std::string& CFLAGS, const std::string& CXXFLAGS, const std::string& JOBS, const std::string& FAKEROOT) : CFLAGS(CFLAGS), CXXFLAGS(CXXFLAGS), JOBS(JOBS), FAKEROOT(FAKEROOT){};
  ~Env();

  int _loadenv_();
};

#endif
