//==========================================|
//   FILE: phrases.cc                       |
//   AUTHOR: Linuxperoxo                    |
//   COPYRIGHT: (c) 2024 per Linuxperoxo.   |
//==========================================/

// Algumas dessas frases foram retiradas de um repo
// LINK: https://github.com/slackjeff/bananapkg 

//==========================================================| LIBS

#include <vector>
#include <string>

//==========================================================| MY HEADERS

#include "random.h"

//==========================================================| FUNCTIONS

std::string Phrases(char type){

  // vector das frases para instalação de pacotes
  const std::vector<std::string> phraseInstall = {

    "Today I just want to install packages! Stop calling me",
    "Wait baby... I install your package, and then we dance.",
    "Wait while Daddy works!",
    "I love my wife",
    "Drunk?? No, thks. I need install a package.",
    "Daddy likes UNIX-LIKE. Please wait I am installing the package.",
    "Wait baby... I install your package, and then we dance.",
    "Come with me if you want to install package.",
    "Zombies are attacking the city. I'll hurry to install the package. Wait."

  };

  // vector das frases para remoção de pacotes
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

  // vector de frases padrão
  const std::vector<std::string> phraseDefault = {

    "I smoke, if I went to eat I would eat.",
    "And here we go. Wait...",
    "Keep it simple stupid >:)",
    "\":3\" = :O 0====3, :===3, :3",
    "Dad is going to work, he's coming!",
    "Daddy is tired but keeps working.",
    "Did you want a daddy like me?",
    "It's a Canso!",
    "Hey UNIX, marry me.",
    "The only road is UNIX.",
    "portage vs lalapkg, portage wins :(",
    "Here we go again. Just a moment...",
    "Free Beer? OMG. :O",
    "rpm vs lalapkg, lalapkg wins"

  };

  switch(tolower(type)){

    case 'i':

      // retornando frase random
      return phraseInstall[randomNum<int>(0, phraseInstall.size() - 1)];

    break;

    case 'r':

      // retornando frase random
      return phraseRemove[randomNum<int>(0, phraseRemove.size() - 1)];

    break;

    default:

      // retornando frase random
      return phraseDefault[randomNum<int>(0, phraseDefault.size() - 1)];

    break;

  }

}
