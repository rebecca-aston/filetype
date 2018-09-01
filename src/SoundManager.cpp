//
//  SoundManager.cpp
//  filetype
//
//  Created by Rebecca Aston on 9/1/18.
//

#include "SoundManager.h"


SoundManager::SoundManager(){
    readTime = -1;
    startTime = ofGetElapsedTimeMillis();
    isReading = false;
}

void SoundManager::update(frame * current){
    
    checkIsReading();
    
    if(!isReading && current->historyVec.size() > 0){ //current.historyVec.size() > 0 && current.currentHistoryIndex < current.historyVec.size() &&
        
       
        if(current->currentHistoryIndex == -1){
            current->currentHistoryIndex = 0;
        }else{
            current->currentHistoryIndex ++;
        }
        
        for(int i = current->currentHistoryIndex; i < current->historyVec.size(); i++){
            if(current->historyVec[i].sound != "") soundFiles.push_back(current->historyVec[i].sound);
            if(current->historyVec[i].length > 0){
                isReading = true;
                readTime = current->historyVec[i].length;
                readText(current->historyVec[current->currentHistoryIndex].text);
                break;
            }else{
                current->currentHistoryIndex ++;
            }
        }
    }
    
    if(soundFiles.size() > 0 && !player.isPlaying()){
        playSound(soundFiles.back());
        soundFiles.pop_back();
    }
}


void SoundManager::playSound(string path){
    player.load(path);
    
    if(player.isLoaded()){
        player.play();
    }else{
        cout << "Wav file cannot be loaded" << endl;
    }
}

void SoundManager::readText(string text){
    //& makes it non-blocking

    startTime = ofGetElapsedTimeMillis();

    std::string open("say '");
    std::string close("' &");
    open += text;
    open += close;
    system(open.c_str());
}

void SoundManager::checkIsReading(){
    if(ofGetElapsedTimeMillis() - startTime > readTime){ // is not reading
        isReading = false;
    }
}
