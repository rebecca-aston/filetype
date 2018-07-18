//
//  Control.h
//
//  Created by Rebecca Aston on 7/17/18.
//
// This handles the loading in of data and the animating of frames by feeding them to the View/Particle System
// This is where the logic for the whole program is handled.

#pragma once

#include "ofMain.h"
#include "frame.h"
#include "JsonRW.h"
#include "PlyRW.h"
#include "BinnedParticleSystem.h"
#include "ofxAssimpModelLoader.h"


class Control {
protected:

    // Binned particle setup
    
    float timeStep;
    float particleNeighborhood, particleRepulsion, particleCohesion, particleAlign; //Hmm align is probably more akin to directional vector
    float centerAttraction;
    float cubeResolution;
    
    int kBinnedParticles, startTime;
    BinnedParticleSystem particleSystem;
    bool isMousePressed, slowMotion;
    
    bool drawBalls;
        
    deque < frame > frames;
    deque < frame* > sequence;
    
    frame currentFrame;
    frame * latestFrame;
    
public:
    Control();
    
    void setup();
    void draw();
    void update();
    void loadFrame();
    void drawStats();
    
    
    
//    If I need to add to GUI
//    ofParameter<float> SIDEWAYS{"Sideways", 0, -1, 1};
};



