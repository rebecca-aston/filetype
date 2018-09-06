#pragma once

#include "BinnedParticle.h"
#include "history.h"

//This maps the JSON data structure to Openframeworks
//It has legacy stuff and aspirational stuff
//I started by writing out a potential data sctructure in a JSON file
//Then set up all the parsing and after that point
//Sort of just experimented in the form I had created
//I will no doubt strip this down and re-shuffle / re-name etc in the future

struct frame {
    
    //initialize with values for logic checks later
    frame(string i = "", int f = -1, bool a = false, bool rM = false, int t = -1, int hI = -1){
        uID = i;
        frameType = f;
        animating = a;
        renderMesh = rM;
        totalTime = t;
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
    
    string uID;
    bool animating;
    bool renderMesh;
    bool addToMeta;
    
    vector< ofVec3f > points;
    vector< ofColor > pointColors;
    
    vector< ofVec3f > pointsB;
    vector< ofColor > pointColorsB;
    
    ofMesh mesh;
    ofImage image;
    

    int newActiveP;
    
    vector< BinnedParticle > particles;

};
