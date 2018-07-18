#pragma once

struct frame {
    string uID;
    int frameType;
    vector< ofVec3f > points;
    vector< ofColor > pointColors;
    bool animating;
    int totalTime;
};
