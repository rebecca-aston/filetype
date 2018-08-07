#pragma once

#include "ofMain.h"

class BinnedParticle {
public:
	BinnedParticle(float x = 0, float y = 0, float z = 0, float xv = 0, float yv = 0, float zv = 0, float l = 100000);
	void updatePosition(float timeStep);
	void resetForce();
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3);// just make resolution?
    void waveFloor(float left, float top, float right, float bottom, float damping = .3);
	void addDampingForce(float damping = .01);
    void align(float _xv, float _yv, float _zv);
    void setTarget(float x, float y, float z);
    void setLife(int l);
	void draw();
    
    float x, y, z;
    float xv, yv, zv;
    float xf, yf, zf;
    float xa, ya, za;
    
    float xt;
    float yt;
    float zt;
    
    ofColor color, targetColor, p1Color, p2Color;
    bool target,triMode;
    float targetDist;
    
    ofVec3f p1, p2;
    
    float life;
}; 
