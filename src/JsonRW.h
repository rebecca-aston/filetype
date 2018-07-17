#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include "Frame.h"
 
class JsonRW {

public:
    JsonRW();
    
    void read(string s);
    void write(Frame frame, string s, bool newFile);
    
    ofxJSONElement json;
    
};

