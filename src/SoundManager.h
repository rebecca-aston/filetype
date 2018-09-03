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
    void addAudioTrack(string path);
    void readText(string text);
    void checkIsReading();
    
    bool isReading;
    
    vector< ofSoundPlayer > audioTracks;
    
public:
    SoundManager();
    
    void updateReadOut(frame * current);
    void updateSound(frame * current);
    void updateVolumes();

};




