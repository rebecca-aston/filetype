#pragma once
#include "ofMain.h"
#include "frame.h"
#include "ofxAssimpModelLoader.h"

class PlyRW {
    
public:
    PlyRW();
    
    frame read(string s, int numParticles);
    void write(Frame frame, string fileName);
    
};
