//==========================================|
//   FILE: locker.cc                        |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <atomic>
#include <thread>

#include "animation.h"
#include "check.h"
#include "locker.h"
#include "color.h"

Locker::Locker(){
  
  if(!check_is_dir(this->lockDir)){
    
    std::filesystem::create_directories(this->lockDir);
    
    this->locked = false;
    
    return;

  }

  if(check_is_file(this->lockDir + this->lockfileName)){
    
    this->locked = true;
    
    return;

  }
  
  this->locked = false;

}

bool Locker::is_Locked() const{
  
  return this->locked;

}

std::string Locker::getFile() const{

  return this->lockDir + this->lockfileName;

}

int Locker::lock(){ 
  
  try{
    
    if(!check_is_file(this->lockDir + this->lockfileName)){
      
      std::ofstream lockfile(this->lockDir + this->lockfileName);
      
      if(check_is_file(this->lockDir + this->lockfileName)){
        
        this->locked = true;

        return EXIT_SUCCESS;

      }  
      
      throw std::runtime_error("An error occurred while trying to lock");

    }
    
    return EXIT_SUCCESS;

  }

  catch(std::runtime_error &error){
    
    std::cout << RED "ERROR: " NC << error.what() << std::endl;
    
    return EXIT_FAILURE;

  }

}

int Locker::unlock(){

  if(this->locked == false){

    return EXIT_SUCCESS;

  } 

  try{

    std::filesystem::remove(this->lockDir + this->lockfileName);

    if(check_is_file(this->lockDir + this->lockfileName)){

      throw std::runtime_error("An error occurred while trying to unlock. If you are not running another lalapkg task, manually remove the file: " GREEN + this->lockDir + this->lockfileName);

    }

    this->locked = false;

    return EXIT_SUCCESS;

  }

  catch(std::runtime_error &error){

    std::cerr << RED "ERROR: " NC << error.what() << std::endl;
    
    return EXIT_FAILURE;

  }

}

void Locker::waiting_Unlock(){
  
  std::atomic<bool> stop = false;

  std::thread animateLocker(animateLoading, std::ref(stop), "Waiting to unlock");

  while(check_is_file(this->lockDir + this->lockfileName)){
    
    std::this_thread::sleep_for(std::chrono::seconds(2)); 

  }

  stop.store(true);

  animateLocker.join();

}

