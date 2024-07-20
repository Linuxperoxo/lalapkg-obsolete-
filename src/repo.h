#ifndef SYNC_REPO_H
#define SYNC_REPO_H

#include <string>

class Repo{

private:

  static const std::string repo_dir;
  static const std::string tmp_dir;
  static const std::string repo_url;

public:

  static int sync();

};

#endif
