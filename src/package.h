#include <string>
#include "check.h" 

#ifndef PACKAGE_H // Guarda de inclusão para evitar múltiplas inclusões acidentais 
#define PACKAGE_H

class Package{
private:
  std::string name;
  std::string version;
  std::string buildfile;
  std::string error;
public:
  Package(std::string name, std::string version, std::string buildfile);
  bool is_inst(std::string world_dir);
  bool makepkg();
  
  std::string getName() const;
  std::string getVersion() const;
  std::string getBuildfile() const;
};

#endif // !PACKAGE_H 
