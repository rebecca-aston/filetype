#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "frame.h"
 
class JsonRW {

public:
    JsonRW();
    
    void read(string s);
    void write(frame f, string s, bool newFile);
    
    ofxJSONElement json;
    
};

