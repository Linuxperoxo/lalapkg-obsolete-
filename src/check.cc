//==========================================|
//   FILE: check.cc                         |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

//==========================================================| LIBS

#include <cstdlib>
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>

//==========================================================| MY HEADERS

#include "check.h"
#include "color.h"
#include "thread"
#include "chrono"

//==========================================================| FUNCTIONS

bool check_is_file(const std::string& file){
  
  return std::filesystem::exists(file) && std::filesystem::is_regular_file(file);

}

bool check_is_dir(const std::string& dir){
  
  return std::filesystem::exists(dir) && std::filesystem::is_directory(dir);

}

int check_dirs(const std::string* dirs[], const int num_dirs){

  for(int i = 0; i < num_dirs; i++){

    // verificando se o diretorio existe
    if(!check_is_dir(*dirs[i])){

      std::cerr << YELLOW << "WARNING: " << NC << "Directory -> " << GREEN << *dirs[i] << NC << " not found" << std::endl;

      // colocando delay antes de criar o diretorio
      std::this_thread::sleep_for(std::chrono::milliseconds(250));

      // criando
      std::filesystem::create_directories(*dirs[i]);  

      std::cout << GREEN ">>> " NC "Created Directory -> " GREEN << *dirs[i] << NC << std::endl;

    }

  }

  return EXIT_SUCCESS;

}

int check_argument(char* arg[], int& num_args, char user_arg[], std::vector<std::string>& packages_vector){
  
  user_arg[0] = '\0';
  
  bool is_sync = false;

  for(int i = 0; i < num_args; i++){
    
    if(arg[i][0] == '-'){
      
      switch(arg[i][1]){

        case 'e':

          // passando argumento
          user_arg[0] = 'e';

          // pegando os pacotes para instalar caso tenha algum
          while(arg[++i] != nullptr){
            
            packages_vector.push_back(arg[i]);

          }

        break;

        case 'u':

          // passando argumento
          user_arg[0] = 'u';

          // pegando os pacotes para remover caso tenha algum
          while(arg[++i] != nullptr){
            
            packages_vector.push_back(arg[i]);

          }
        
        break;

        case 's':

          // passando argumento
          user_arg[0] = 's';

          // deixando como true para não cair no if packages_vector.empty() 
          is_sync = true;

        break;

        // case para pegar informações do pacote
        case 'i':

          user_arg[0] = 'i';
          user_arg[1] = arg[i][2];

          if(arg[i + 1] != nullptr){
            
            packages_vector.push_back(arg[++i]);

          }
        
        break;

        default:
          
          std::cerr << RED << "ERROR: " << NC << "Invalid argument: " << GREEN << arg[i] << NC << std::endl;  
          
          return EXIT_FAILURE;
          
        break;

      }

    }

  }

  // caso o user não passe nenhum argumento
  if(user_arg[0] == '\0'){
    
    std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "argument" << NC << std::endl;
    
    return EXIT_FAILURE;

  }

  if(!is_sync){

    // caso o user não passe nenhum pacote para ser instalado
    if(packages_vector.empty()){
      
      std::cerr << RED << "ERROR: " << NC << "U must specify some " << GREEN << "package" << NC << std::endl;
      
      return EXIT_FAILURE;

    }

  }

  return EXIT_SUCCESS;

}

bool checkCommand(const std::string command){

  // verificando se o comando existe usando o which
  return !system(("which " + command + " &> /dev/null").c_str());

}
