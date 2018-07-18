#pragma once

struct frame {
    string uID;
    string frameType;
    vector< ofVec3f > points;
    vector< ofColor > pointColors;
    bool animating;
    float totalTime;
};
