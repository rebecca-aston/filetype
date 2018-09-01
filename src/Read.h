#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxAssimpModelLoader.h"
#include "frame.h"
#include "history.h"
 
class Read {

public:
    Read();
    
    void readModel();
    vector< frame > readJson(string path); // return type?
    ofMesh readMesh(string path);
//    ofSound readSound(string path);
    string readText(string path);
    ofImage readImage(string path);
    
    ofDirectory modelData;
    vector< frame > frameVec;
    bool dev;
    
};

