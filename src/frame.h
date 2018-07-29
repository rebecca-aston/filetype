#pragma once

struct frame {
    
    //initialize with values for logic checks later
    frame(string i = "", int f = -1, bool a = false, int t = -1){
        uID = i;
        frameType = f;
        animating = a;
        totalTime = t;
    }
    
    string uID;
    int frameType;
    bool animating;
    int totalTime;
    
    vector< ofVec3f > points; //default constructed to be empty
    vector< ofColor > pointColors;

};
