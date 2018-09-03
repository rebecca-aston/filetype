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
    
    ofImage rouleteImage;
    vector<frame> drawFrames;
    
    ofCamera cam;
    
    int colWidth;
    ofVec2f imgSectionOrigin;
    
public:
    DataScreen();
    
    void setup();
    void draw();
    
    string wrapString(string text, int width);
    
    void drawImageRoulette(ofImage i);
    void loadData(frame current);
    void removeFrame(int t);

};




