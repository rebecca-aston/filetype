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
    void clearVectors();
    
    vector< frame > readJson(string path);
    ofMesh readMesh(string path);
    string readText(string path);
    ofImage readImage(string path);
    
    ofDirectory modelData;
    vector< frame > imageFrameVec;
    vector< frame > threeDFrameVec;
    vector< frame > textFrameVec;
    vector< frame > processFrameVec;
    bool dev;
    
};

