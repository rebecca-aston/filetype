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
#include "BinnedParticleSystem.h"


class DataScreen {
protected:
    ofTrueTypeFont font;
    
    frame newestFrame;
    frame imageFrame;
    frame textFrame;
    frame meshFrame;
    
    ofCamera camBirdsEye;
    
    float colWidth;
    
    void drawSection(frame f, float x, float y, float colW, float colH);
    void updateMeta();
    void textLayout(frame f, float x, float y, float colW, float colH);
    void scaleImageSquare(ofImage img, float size, float padding);
    void removeFrame(int t);
    string wrapString(string text, int width);
   
    float imgPadding;
    float lineHeight;
    float textPadding;
    
public:
    DataScreen();
    
    void setup();
    void draw(BinnedParticleSystem * p, BinnedParticleSystem * b);
    void loadData(frame current);
    
};




