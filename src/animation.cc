//==========================================|
//   FILE: animation.cc                     |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <cctype>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>

#include "animation.h"
#include "color.h"
#include "phrases.h"


void animate(std::atomic<bool>& done, std::string msg, char type){

  const std::vector<std:string> animation[] = {
    
      "c o o o o o o o o o",
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
      "---------------c o o",
      "---------------C o o",
      "-----------------c o",
      "-----------------C o",
      "-------------------c",
      "-------------------C",
      "--------------------"

  };
  
  const std::string phrase = Phrases(type);

  const int delay = 150;

  std::cout << msg << std::endl;

  while(!done){

    for(const std::vector<std::string>& frames : animation){

      std::cout << "\r" GREEN "* " NC << phrase << NC BLUE " [" NC << frames << BLUE "]" NC;
      std::cout.flush();

      if(done){

        break;

      }

      std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    }    

  }

  std::cout << std::endl;

}
