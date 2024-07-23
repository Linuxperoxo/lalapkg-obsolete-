//==========================================|
//   FILE: animation.cc                     |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

//==========================================================| LIBS

#include <cctype>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <vector>

//==========================================================| MY HEADERS

#include "animation.h"
#include "color.h"
#include "phrases.h"

//==========================================================| FUNCTIONS

void animate(std::atomic<bool>& done, std::string msg, char type){

  const std::vector<std::string> animation = {
    
      // Vetor de cada frame da animação 
      "o o o o o o o o o o",
      "C o o o o o o o o o",
      "--c o o o o o o o o",
      "--C o o o o o o o o",
      "----c o o o o o o o",
      "----C o o o o o o o",
      "------c o o o o o o",
      "------C o o o o o o",
      "--------c o o o o o",
      "--------C o o o o o",
      "----------c o o o o",
      "----------C o o o o",
      "------------c o o o",
      "------------C o o o",
      "--------------c o o",
      "--------------C o o",
      "----------------c o",
      "----------------C o",
      "------------------c",
      "------------------C",
      "-------------------"

  };
  
  // Pegando uma frase aleatória
  const std::string phrase = Phrases(type);

  // Ajustando delay de troca dos frames 
  const int delay = 150;

  // Mostrando mensagem passada na função 
  std::cout << msg << std::endl;

  // While responsável por manter rodando a animação 
  while(!done){

    // interando o vetor das animações 
    for(const std::string& frames : animation){

      std::cout << "\r" GREEN "* " NC << phrase << NC BLUE " [" NC << frames << BLUE "]" NC;
      std::cout.flush();

      // Verificando se a tarefa foi concluída.
      // Caso não tenha essa verificação vai passar por todos os frames antes de sair
      if(done){

        break;

      }

      // Timer para trocar de frame
      std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    }    

  }

  // Quebrando a linha para a saída ficar mais bonitinha :D
  std::cout << std::endl;

}
