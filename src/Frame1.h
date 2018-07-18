//
//  Frame.h
//
//  Created by Rebecca Aston on 7/17/18.
//
// This is the container of data for animating the particle system/view of the project.
// Designed to be instantiated inside of vectors which are then cycled through.
// JSON file data will be loaded into here and written from here.
// This is the equivalent of one STATE or one MESH.

#pragma once

#include "ofMain.h"

class Frame {
protected:
    string frameType;
    vector< ofVec3f > points;
    vector< ofColor > pointColors;
    bool animating;
    float totalTime;
    
public:
    Frame();
    
    //maybe think about how you could add branching data in here
    //textual data
    //sound data
    //etc
    
    string getType();
    void setType(string t);
    
    vector< ofVec3f > getPoints();
    void setPoints(vector< ofVec3f > c);
    void addPoint(ofVec3f c);
    
    vector< ofColor > getPointColors();
    void setPointColors(vector< ofColor > p);
    void addPointColor(ofColor p);
    
    
};


