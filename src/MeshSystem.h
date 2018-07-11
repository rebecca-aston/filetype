#pragma once
#include "ofMain.h"
#include "Vert.h"
#include "ofxAssimpModelLoader.h"

class MeshSystem{
    
public:
    MeshSystem();

    void setup(int _initResolution, int _numParticles, string loadModel);
    void update(ofVec3f attractor);
    void draw(bool wireFrame);
    void pushVert(ofVec3f pos, int i, ofColor _color);
    void exportMesh(string fileName);
    
    //Slides
    ofParameter<float> SIZE{"Size", 5, 0, 50};
    ofParameter<float> PROBABILITY{"Probability", 0.95, 0, 1};
    ofParameter<float> SPEED{"Speed to Attractor", 0.4, 0, 5};
    ofParameter<float> GRAVITY{"Gravity", 0, -1, 1};
    ofParameter<float> CHECKDIST{"Distance to Check", 0, -1, 1};
    ofParameter<float> SIDEWAYS{"Sideways", 0, -1, 1};
    
    ofxAssimpModelLoader model;
    
    int initResolution, numParticles;
    
    ofMesh *meshPointer;
    
    vector<Vert*> vertices;
    
};
