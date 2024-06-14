#include <string>
#include <cstdlib>
#include "package.h"

Package::Package(std::string name, std::string version, std::string buildfile){
  this->name = name;
  this->version = version;
  this->buildfile = buildfile;
}

bool Package::is_inst(std::string world_dir){
  if(check_file(world_dir + "/" + this->name + "-" + this->version)){
    return true;
  }
  return false;
}

bool Package::makepkg(){
  if(system("command -v check_script") == 0){
    system("./check_script");
  }
  return false;
}

std::string Package::getName() const{
  return this->name;
}

std::string Package::getVersion() const{
  return this->version;
}

std::string Package::getBuildfile() const{
  return this->buildfile;
}
