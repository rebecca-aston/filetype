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

void SoundManager::updateReadOut(frame * current){
    checkIsReading();
    
    if(!isReading && current->historyVec.size() > 0){ //current.historyVec.size() > 0 && current.currentHistoryIndex < current.historyVec.size() &&
        
       
        if(current->currentHistoryIndex == -1){
            current->currentHistoryIndex = 0;
        }else{
            current->currentHistoryIndex ++;
        }
        
        for(int i = current->currentHistoryIndex; i < current->historyVec.size(); i++){
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

}

void SoundManager::readText(string text){
    //& makes it non-blocking
    
    startTime = ofGetElapsedTimeMillis();
    
    std::string open("say '");
    std::string close("' &");
    open += text;
    open += close;
//    system(open.c_str());
}

void SoundManager::checkIsReading(){
    if(ofGetElapsedTimeMillis() - startTime > readTime){ // is not reading
        isReading = false;
    }
}

void SoundManager::updateSound(frame * current){
    for(int i = 0; i < current->historyVec.size(); i++){
        if(current->historyVec[i].sound != "") addAudioTrack(current->historyVec[i].sound);
    }
}

void SoundManager::addAudioTrack(string path){
    ofSoundPlayer sp;
    sp.load(path);
    
    cout << path << endl;
    
    if(sp.isLoaded()){
        audioTracks.push_back(sp);
        audioTracks.back().setVolume(1.2);
        audioTracks.back().play();
    }else{
        cout << "File cannot be loaded" << endl;
    }
}

void SoundManager::updateVolumes(){
    for(int i = 0; i < audioTracks.size();i++){
        if(i != audioTracks.size()-1 && audioTracks[i].getVolume() > 0.5){
            audioTracks[i].setVolume(audioTracks[i].getVolume()-0.01);
        }
        if(!audioTracks[i].isPlaying()){
            audioTracks.erase(audioTracks.begin()+i);
        }
    }
}


