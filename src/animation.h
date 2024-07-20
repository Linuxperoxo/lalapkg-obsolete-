//==========================================|
//   FILE: animation.h                      |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <atomic>
#include <string>

void animate(std::atomic<bool> &done, std::string msg, char type = 'z');

#endif 

