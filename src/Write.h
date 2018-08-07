#pragma once
#include "ofMain.h"
#include "frame.h"
#include "ofxJSON.h"


class Write {
    
public:
    Write();
    
    void writeMesh(frame f, string fileName);
    void writeJson(frame f, string s, bool newFile);
    
};
