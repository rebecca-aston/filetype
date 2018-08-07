#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxAssimpModelLoader.h"
#include "frame.h"
 
class Read {

public:
    Read();
    
    frame readJson(string s); // return type?
    ofMesh readMesh(string s);
    
    ofxJSONElement data;
};

