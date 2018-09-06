//
//  Control.h
//
//  Created by Rebecca Aston on 7/17/18.
//

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

    // Binned particle
    float timeStep;
    float particleNeighborhood, particleRepulsion, particleCohesion, particleAlign; 
    float centerAttraction;
    float cubeResolution;
    
    int startTime;
    BinnedParticleSystem particleSystem;
    BinnedParticleSystem backBurnerSystem;

    Read read;
    Write write;
    SoundManager soundManager;
    frame scatter;

    bool scattering;
   
public:
    Control();
    
    void setup();
    void draw();
    void update();
    void loadFrame(Sequencer * sequence); 
    void addMeshToParticleSys(ofMesh mesh);
    void addMeshToBackBurnerSys(ofMesh mesh);
    
    void setupDataScreen();
    void drawDataScreen();
    
    DataScreen dataScreen;
    Sequencer* getSequenceByType(string t);
    
    void exportPLY();
    
    vector<Sequencer> sequences;
    
    ofVec3f camLook;
    int randParticle;
    
};



