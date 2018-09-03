//
//  Sequencer.h
//
//  Created by Rebecca Aston on 7/17/18.
//
// This handles the loading in of data and the animating of frames by feeding them to the View/Particle System
// This is where the logic for the whole program is handled.

#pragma once

#include "ofMain.h"
#include "frame.h"
#include "history.h"

class Sequencer {
protected:

    
   
public: 
    Sequencer();
    
    void setup(vector<frame> vecRef, string type);
    void update();
    
    void resetCurrentFrame();
    void sequencer();
    
    string vecType;
    frame currentFrame;
    vector < frame > frameVec;
    
    int startTime;
    int weightedRandom;
};



