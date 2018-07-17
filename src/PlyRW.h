#pragma once
#include "ofMain.h"
#include "Frame.h"
#include "ofxAssimpModelLoader.h"

class PlyRW {
    
public:
    PlyRW();
    
    Frame read(string s, int numParticles);
    void write(Frame frame, string fileName);
    
};
