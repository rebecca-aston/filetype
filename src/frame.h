#pragma once

#include "BinnedParticle.h"

struct frame {
    
    //initialize with values for logic checks later
    frame(string i = "frame"+ofGetTimestampString(), int f = -1, bool a = false, bool rM = false, int t = -1, int l = -1){
        uID = i;
        frameType = f;
        animating = a;
        renderMesh = rM;
        totalTime = t;
        leader = l;
    }
    
    string uID;
    int frameType;
    bool animating;
    bool renderMesh;
    int totalTime;
    
    //Might not use these
    vector< ofVec3f > points; //default constructed to be empty
    vector< ofColor > pointColors;
    
    ofMesh mesh;
    
    int leader;
    int newActiveP;
    
    vector< BinnedParticle > particles;

};
