#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>

struct Config_file{
  std::string sync;
  std::string source_dir;
  std::string pkg_dir;
  std::string root_dir;
  std::string common_flags;
  std::string jobs;
};

int load_config(const std::string& file, Config_file* conf_file);

#endif
