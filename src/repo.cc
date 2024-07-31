#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <chrono>
#include <atomic>
#include <thread>
#include <filesystem>

#include "animation.h"
#include "color.h"
#include "repo.h"
#include "check.h"

const std::string Repo::repo_dir = "/var/lalapkg/repo";
const std::string Repo::tmp_dir = "/tmp/lalapkg/repo";
const std::string Repo::repo_url = "https://github.com/Linuxperoxo/repo.git";

int Repo::sync(){

  if(!check_is_dir(tmp_dir)){

    std::filesystem::create_directories(tmp_dir);

  }

  if(!check_is_dir(repo_dir)){

    std::filesystem::create_directories(repo_dir);

  }

  if(!std::filesystem::is_empty(repo_dir)){

    if(system(("cd " + repo_dir + " && tar czf " + tmp_dir + "/repo_backup.tar.gz * &> /dev/null").c_str()) != 0){

      std::cerr << YELLOW "WARNING: " NC "Unable to create backup of local repository" << std::endl;
      
      std::this_thread::sleep_for(std::chrono::milliseconds(800));

    } else {

      std::cerr << GREEN ">>> " NC "Repository backup create in " GREEN << tmp_dir << "/repo_backup.tar.gz" NC << std::endl;

    }

    std::filesystem::remove_all(repo_dir);

  }

  std::atomic<bool> stop = false;

  std::thread animationSync(animate, std::ref(stop), GREEN "--- " NC "Synchronizing repository: " GREEN + repo_url + NC, 'z');

  int result = system(("git clone " + repo_url + " " + repo_dir + " &> /dev/null").c_str());

  stop.store(true);

  animationSync.join();

  if(result != 0){

    std::cout << RED "ERROR: " NC "Error when cloning repository: " GREEN << repo_url << NC << std::endl;

    return EXIT_FAILURE;

  }

  std::cerr << GREEN ">>> " NC << "Repository synchronized successfully!" << std::endl;

  return EXIT_SUCCESS;

}

