/*

/==========================================|
|   FILE: main.cc                          |
|   VERSION: 1.1.2a                        |
|   AUTHOR: Linuxperoxo                    |
|   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
|==========================================/

|=====================================================================|
| DESCRIÇÂO |                                                         |
|------------                                                         |
|                                                                     |
|   Um gerenciador de pacotes simples, focado no minimalismo e na     |
| simplicidade.                                                       |
|                                                                     |
| OBS:                                                                |
|   Este pequeno projeto foi desenvolvido por pura diversão e como    | 
| uma forma de praticar minha lógica de programação, visando          |
| aprimorar gradualmente minhas habilidades na linguagem C++          |
|=====================================================================|

|=====================================================================|
| CHANGE LOG |                                                        |
|-------------                                                        |
|                                                                     |
|1.1.1a:                                                              |
| * Melhorias no código;                                              |
| * Agora a base dos argumentos principais foram adicionados;         |
| * Novo .h "package_operations.h";                                   |
| * Adicionado suporte a instalação enfileirada de pacotes;           |
|                                                                     |
|1.1.2a:                                                              |
| * Correção de bugs;                                                 |
| * Novo .h "package.h";                                              |
| * Os argumentos foram removidos no momento;                         |
| * Novo arquivo no repositorio "Pkg-infos";                          |
| * Função package_exist foi adaptada;                                |
| * Classe para pacotes foi adicionado para melhor organização e      |
|   Manipulação do pacote;                                            |
| * Nova função adicionada em package_operations;                     |
|                                                                     |
|1.1.2-1a:                                                            |
| * Correção de bugs;                                                 |
| * Função parse_arguments melhorada;                                 |
| * Destrutor da classe Package adicionado;                           |
| * Melhorias na eficiência do código;                                |
| * Função get_infos adicionada                                       |
|=====================================================================|

|=====================================================================|
| TO DOS |                                                            |
|---------                                                            |
| RUNNING: Adicionar funções na classe Package para manipulação       |
| do pacote;                                                          |
| * Primeiros pacotes serão adicionados ao repositorio para testar    |
|   Funções básicas do gerenciador;                                   |
|=====================================================================| 

Esse commit ainda está incompleto, mas os códigos que foram adicionadas
estão funcionando como esperado. Na teoria a parte da compilação está
funcionando muito bem, mas faltam alguns teste para comprovar 100%
seu funcionamento. A instalação do pacote ainda não está sendo efetuada, 
mas o código original já possue a instalação no estágio beta.

*/

//==========================================================| LIBS

#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <libconfig.h++>
#include <filesystem>

//==========================================================| MY HEADERS

#include "check.h"
#include "package_operations.h"
#include "package.h"
#include "color.h"

//==========================================================| CONST VARS

const std::string config_file = "/etc/lala.conf";
const std::string repo_dir = "/var/lalapkg/repo";
const std::string info_file = "Infopkg";
const std::string build_file = "Buildpkg";
const std::string world_dir = "/var/lalapkg/world/";

//==========================================================| PACKAGE VECTOR

std::vector<std::string> packages_vector;

//==========================================================| CONFIG_FILE STRUCT 

struct Config_file{
  std::string sync;
  std::string source_dir;
  std::string root_dir;
  std::string installbin_dir;
  std::string common_flags;
  std::string jobs;
};

Config_file* conf_file = new Config_file;

//==========================================================| FUNCTIONS

int load_config(const std::string& file){
  libconfig::Config conf_obj;
  
  try{
    conf_obj.readFile(file);
    
    const std::string var_names[] = {"sync", "source_dir", "root_dir", "installbin_dir", "common_flags", "jobs"};
    std::string* var_pts[] = {&conf_file->sync, &conf_file->source_dir, &conf_file->root_dir, &conf_file->installbin_dir, &conf_file->common_flags, &conf_file->jobs};
    
    const size_t size = sizeof(var_pts) / sizeof(var_pts[0]);

    bool ERROR = false;

    std::cout << GREEN << "|==========VAR-RESULTS=========|" << NC << std::endl;
    
    for(int i = 0; i < size; i++){
      conf_obj.lookupValue(var_names[i], *var_pts[i]);
      if(var_pts[i]->empty()){
        if(var_names[i] == "common_flags" || var_names[i] == "jobs"){
          std::cerr << var_names[i] << "...   [ " << YELLOW << "WARNING" << NC << " ]" << std::endl;
        } else {
          ERROR = true;
          std::cerr << var_names[i] << "...   [ " << RED << "FAILED" << NC << " ]" << std::endl;
        }
      } else {
        std::cerr << var_names[i] << "...   [ " << GREEN << "OK"<< NC << " ]" << std::endl;
      }
    }

    std::cout << GREEN << "|==============================|" << NC << std::endl;

    if(ERROR){
      throw std::runtime_error("Error while trying to load some var: Check config file -> " GREEN + config_file + NC);
    }
    return EXIT_SUCCESS;
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch(libconfig::ParseException &paex){
    std::cerr << RED << "ERROR: " << NC <<  "Parse error in build file -> " << GREEN << paex.getFile() << NC << " -> " << RED << paex.getError() << NC << " -> " << "line -> " << RED << paex.getLine() << NC << std::endl;
    return EXIT_FAILURE;
  }

  catch(libconfig::FileIOException &fioex){
    std::cerr << RED << "ERROR: " << NC << "I/O error reading build file -> " << RED << fioex.what() << NC << std::endl;
    return EXIT_FAILURE;
  }
}

int check_dirs(const std::string* dirs[], const std::string& warning_dir, const int num_dirs){
  for(int i = 0; i < num_dirs; i++){
    if(!check_is_dir(*dirs[i])){
      if(*dirs[i] == warning_dir){
        std::cerr << YELLOW << "WARNING: " << NC << "repository directory -> " << GREEN << *dirs[i] << NC << " does not exist, use" << GREEN << " lalapkg --sync" << NC << std::endl;
      } else {
        std::cerr << YELLOW << "WARNING: " << NC << "Directory -> " << GREEN << *dirs[i] << NC << " not found" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        
        try{
          std::filesystem::create_directories(*dirs[i]);
          std::cout << ">>> Created Directory -> " << GREEN << *dirs[i] << NC << std::endl;
        }

        catch(std::filesystem::filesystem_error &error){
          std::cerr << RED << "ERROR: " << NC << "Failed to create directory -> " << GREEN << *dirs[i] << ": " << NC << error.what() << std::endl; 
          return EXIT_FAILURE;
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

char parse_arguments(char* arg[], int& num_args, char& user_arg){
  int packages_founds = 0;

  for(int i = 0; i < num_args; i++){
    if(arg[i][0] == '-'){
      switch(arg[i][1]){
        case 'e':
          user_arg = 'e';

          while(arg[++i] != nullptr){
            packages_vector.push_back(arg[i]);
            packages_founds++;
          }
        break;

        case 'u':
          user_arg = 'u';

          while(arg[++i] != nullptr){
            packages_vector.push_back(arg[i]);
            packages_founds++;
          }
        break;

        default:
          std::cerr << RED << "ERROR: " << NC << "Invalid argument: " << GREEN << arg[i] << NC << std::endl;  
          return EXIT_FAILURE;
        break;
      }
    }
  }

  if(user_arg == '\0'){
    std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "argument" << NC << std::endl;
    return EXIT_FAILURE;
  }

  if(packages_founds > 0){
    return EXIT_SUCCESS;
  }

  std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "package" << NC << std::endl;
  return EXIT_FAILURE;
}

int emerge(std::string pkg){
  const std::string pkgroot = package_exist(repo_dir, pkg, build_file, info_file);

  if(pkgroot == ""){
    return EXIT_FAILURE;
  }

  Package* newpkg = nullptr;

  try{
    Package* newpkg = new Package(pkgroot + info_file, pkgroot + build_file);

    newpkg->makepkg(conf_file->source_dir, conf_file->common_flags, conf_file->jobs);
    newpkg->installpkg(conf_file->installbin_dir, world_dir, conf_file->source_dir);
  }

  catch(std::runtime_error &error){
    std::cerr << RED << "ERROR: " << NC << error.what() << std::endl;
    std::cerr << RED << "ERROR: " << pkg << NC << " package failure!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main_switch_loop(char& user_arg){
  switch(user_arg){
    case 'e':
      for(const auto& vector : packages_vector){
        emerge(vector);
      }
    break;

    case 'u':
      for(const auto& vector : packages_vector){
        // unmerge
      }
    break;
  }
  return EXIT_FAILURE;
}

//==========================================================| MAIN

int main(int argc, char* argv[]){
  const std::string* check_this_dirs[] = {&conf_file->source_dir, &conf_file->root_dir, &conf_file->installbin_dir, &repo_dir, &world_dir}; 
  const size_t size = sizeof(check_this_dirs) / sizeof(check_this_dirs[0]);
  
  char arg;
  
  if(load_config(config_file) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(check_dirs(check_this_dirs, repo_dir, size) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(parse_arguments(argv, argc, arg) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  if(main_switch_loop(arg) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

