#include <vector>
#include <string>

#include "random.h"

std::string Phrases(char type){

  const std::vector<std::string> phraseInstall = {

    "Today I just want to install packages! Stop calling me",
    "Wait baby... I install your package, and then we dance.",
    "Wait while Daddy works!",
    "Its a Canso!",
    "I love my wife",
    "Drunk?? No, thks. I need install a package.",
    "Daddy likes UNIX-LIKE. Please wait I am installing the package.",
    "Wait baby... I install your package, and then we dance.",
    "Come with me if you want to install package.",
    "Zombies are attacking the city. I'll hurry to install the package. Wait."

  };

  const std::vector<std::string>phraseRemove = {

    "Package, u accept marry me? No!? Ok, remove now! UwU MUAHAHAHAH",
    "Hold tight, I'm uninstalling those packages!",
    "Captain! The boat can not sink, pull the anchor and go packing",
    "Welcome to the Jungle... Baby!",
    "Time to bid farewell, little package!",
    "Package, it's been good while it lasted. Now it's time to completely remove you.",
    "Package, your stay is over. I'm removing you now.",
    "Cleaning the house for when dad arrives. ^_^"

  };


  const std::vector<std::string> phraseDefault = {

    "I smoke, if I went to eat I would eat.",
    "And here we go. Wait...",
    "Dad is going to work, he's coming!",
    "Daddy is tired but keeps working.",
    "Did you want a daddy like me?",
    "Hey UNIX, marry me.",
    "The only road is UNIX.",
    "portage vs lalapkg, portage wins :(",
    "Here we go again. Just a moment...",
    "Free Beer? OMG.",
    "rpm vs lalapkg, lalapkg wins"

  };

  switch(tolower(type)){

    case 'i':

      return phraseInstall[randomNum<int>(0, phraseInstall.size() - 1)];

    break;

    case 'r':

      return phraseRemove[randomNum<int>(0, phraseRemove.size() - 1)];

    break;

    default:

      return phraseDefault[randomNum<int>(0, phraseDefault.size() - 1)];

    break;

  }

}
