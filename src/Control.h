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
#include "history.h"
#include "Read.h"
#include "Write.h"
#include "BinnedParticleSystem.h"
#include "ofxAssimpModelLoader.h"
#include "DataScreen.h"
#include "DelaunayImage.h"
#include "SoundManager.h"
#include "Sequencer.h"

class Control {
protected:

    // Binned particle setup
    
    //PROBABLY DON'T NEED half of these
    float timeStep;
    float particleNeighborhood, particleRepulsion, particleCohesion, particleAlign; 
    float centerAttraction;
    float cubeResolution;
    
    int kBinnedParticles, startTime;
    BinnedParticleSystem particleSystem;
    BinnedParticleSystem backBurnerSystem;
    bool isMousePressed, slowMotion;
    
    bool drawBalls;
    
    Read read;
    Write write;
    
//    frame flock;
    
    SoundManager soundManager;
    
    bool scattering;
    int scatterSource;
   
public:
    Control();
    
    void setup();
    void draw();
    void update();
    void loadFrame(Sequencer * sequence); 
    void addMeshToParticleSys(ofMesh mesh);
    
    void setupDataScreen();
    void drawDataScreen();
    
    void exportPLY();
    
    vector<Sequencer> sequences;
    
    DelaunayImage imgRoulete;
    
    ofVec3f camLook;
    int randParticle;
    
    DataScreen dataScreen;
    
    
//    If I need to add to GUI
//    ofParameter<float> SIDEWAYS{"Sideways", 0, -1, 1};
};



