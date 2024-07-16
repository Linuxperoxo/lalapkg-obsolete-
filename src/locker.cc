#include <chrono>
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
  
  if(!check_is_dir(this->lockdir)){
    
    std::filesystem::create_directories(this->lockdir);
    
    this->Locked = false;
    
    return;

  }

  if(check_is_file(this->lockdir + this->lockfile_name)){
    
    this->Locked = true;
    
    return;

  }
  
  this->Locked = false;

}

bool Locker::is_Locked() const{
  
  return this->Locked;

}

int Locker::lock_Process(){ 
  
  try{
    
    if(!check_is_file(this->lockdir + this->lockfile_name)){
      
      std::ofstream lockfile(this->lockdir + this->lockfile_name);

      if(check_is_file(this->lockdir + this->lockfile_name)){
        
        this->Locked = true;

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

void Locker::waiting_Unlock(){
  
  std::atomic<bool> done = false;

  std::thread animateLocker(animateLoading, std::ref(done), "Waiting to unlock");

  while(check_is_file(this->lockdir + this->lockfile_name)){
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
  }

  done.store(true);

  animateLocker.join();

}

