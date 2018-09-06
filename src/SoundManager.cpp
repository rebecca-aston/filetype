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
    
    if(!isReading && current->historyVec.size() > 0){
       
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

    startTime = ofGetElapsedTimeMillis();
    
    std::string open("say '");
    std::string close("' &");
    open += text;
    open += close;
    
     //THIS!!! Was causing my program to mysteriously crash after 45 / 60 mins. I left the logic flow in
    //as it was a last minute discovery sadly and just so you can see how I intended it
    //I will implement a better programmatic text to speech
    //For the show I just went analog and used an online converter to make a more curated selection of text
    //Included a crash/error image as it only ever even managed to get all the way through debugger error throwing once
    //It usually just froze
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
        if(i != audioTracks.size()-1 && audioTracks[i].getVolume() > 0.2){
            audioTracks[i].setVolume(audioTracks[i].getVolume()-0.01);
        }
        if(!audioTracks[i].isPlaying()){
            audioTracks.erase(audioTracks.begin()+i);
        }
    }
}


