//==========================================|
//   FILE: animation.cc                     |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "animation.h"
#include "color.h"

void animateLoading(std::atomic<bool>& done, std::string msg){

  const char animationChar[5] = {'/', '-', '\\', '-', '|'};
  
  int i = 0;

  while(!done){

    std::cout << "\r" << msg << " " << "[" << GREEN << animationChar[i] << NC << "]";
    
    std::cout.flush();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    
    i = (i + 1) % sizeof(animationChar);

  }
  
  std::cout << std::endl;

}
