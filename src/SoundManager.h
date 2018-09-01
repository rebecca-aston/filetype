//
//  Control.h
//
//  Created by Rebecca Aston on 7/17/18.
//


#pragma once

#include "ofMain.h"
#include "frame.h"
#include "history.h"

class SoundManager {
protected:
    
    int startTime, readTime;
    void playSound(string path);
    void readText(string text);
    void checkIsReading();
    
    bool isReading;
    
    ofSoundPlayer player;
    vector<string> soundFiles;
    
public:
    SoundManager();
    
    void update(frame * current);
    

};




