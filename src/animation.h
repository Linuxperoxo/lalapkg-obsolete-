//==========================================|
//   FILE: animation.h                      |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef ANIMATION_H
#define ANIMATION_

//==========================================================| LIBS

#include <atomic>
#include <string>

// Declarando função 
void animate(std::atomic<bool> &done, std::string msg, char type = 'z');

#endif 

