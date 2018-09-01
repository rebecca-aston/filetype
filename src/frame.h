#pragma once

#include "BinnedParticle.h"
#include "history.h"

struct frame {
    
    //initialize with values for logic checks later
    frame(string i = "frame"+ofGetTimestampString(), int f = -1, bool a = false, bool rM = false, int t = -1, int l = -1, int hI = -1){
        uID = i;
        frameType = f;
        animating = a;
        renderMesh = rM;
        totalTime = t;
        leader = l;
        currentHistoryIndex = hI;
    }
    

    //larger meta collection and authoring data
    string collection;
    string collectionDesc;
    int encounterDate;
    float encounterLocLong;
    float encounterLocLat;
    string author;
    string citation;
    
    //frame data
    int frameType;
    float weight;
    vector<string> tags;
    string title;
    string desc;
    string material;
    string externalFileName;
    int totalTime;
    vector< history > historyVec;
    int currentHistoryIndex;
    
    //programmatic stuff
    string uID;
    bool animating;
    bool renderMesh;
    
    vector< ofVec3f > points; //default constructed to be empty
    vector< ofColor > pointColors;
    
    vector< ofVec3f > pointsB; //default constructed to be empty
    vector< ofColor > pointColorsB;
    
    ofMesh mesh;
    ofImage image;
    
    int leader; //Flocking
    int newActiveP;
    
    vector< BinnedParticle > particles;

};
