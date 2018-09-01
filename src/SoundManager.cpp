//
//  SoundManager.cpp
//  filetype
//
//  Created by Rebecca Aston on 9/1/18.
//

#include "SoundManager.h"


SoundManager::SoundManager(){
    
}

void SoundManager::readText(string text){
    //& makes it non-blocking
//    system("say '"+text+"' &");
    std::string open("say '");
    std::string close("' &");
    open += text;
    open += close;
    system(open.c_str());
}
