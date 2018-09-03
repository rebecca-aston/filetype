//
//  DataScreen.h
//
//  Created by Rebecca Aston on 7/17/18.
//
// This handles the loading in of data and the animating of frames by feeding them to the View/Particle System
// This is where the logic for the whole program is handled.

#pragma once

#include "ofMain.h"
#include "frame.h"


class DataScreen {
protected:
    ofTrueTypeFont font;
    
    frame currentFrame;
    
    ofImage rouleteImage;
    
public:
    DataScreen();
    
    void setup();
    void draw();
    
    void drawImageRoulette(ofImage i);
    void loadData(frame current);

};




