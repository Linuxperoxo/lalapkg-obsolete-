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

const std::string Locker::lockDir = "/var/lalapkg/locker/";
const std::string Locker::lockfileName = "lockfile";

bool Locker::locked = false;

bool Locker::is_Locked() {
  
  if(!check_is_dir(lockDir)){

    std::filesystem::create_directories(lockDir);

    locked = false;

    return locked;

  }

  if(check_is_file(lockDir + lockfileName)){

    locked = true;

  } 

  return locked;

}

std::string Locker::getFile() {

  return lockDir + lockfileName;

}

int Locker::lock(){ 
  
  try{
    
    if(!check_is_file(lockDir + lockfileName)){
      
      std::ofstream lockfile(lockDir + lockfileName);
      
      if(check_is_file(lockDir + lockfileName)){
        
        locked = true;

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

  if(locked == false){

    return EXIT_SUCCESS;

  } 

  try{

    std::filesystem::remove(lockDir + lockfileName);

    if(check_is_file(lockDir + lockfileName)){

      throw std::runtime_error("An error occurred while trying to unlock. If you are not running another lalapkg task, manually remove the file: " GREEN + lockDir + lockfileName);

    }

    locked = false;

    return EXIT_SUCCESS;

  }

  catch(std::runtime_error &error){

    std::cerr << RED "ERROR: " NC << error.what() << std::endl;
    
    return EXIT_FAILURE;

  }

}

void Locker::waiting_Unlock(){
  
  while(check_is_file(lockDir + lockfileName)){
    
    std::cout << "\r" GREEN "*" NC " Waiting for daddy to unlock the door";
    std::cout.flush();
    
    std::this_thread::sleep_for(std::chrono::seconds(2)); 

  }

  std::cout << std::endl;  

}

