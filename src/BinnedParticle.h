#pragma once

#include "ofMain.h"

class BinnedParticle {
public:
	float x, y, z;
	float xv, yv, zv;
	float xf, yf, zf;
	BinnedParticle(float x = 0, float y = 0, float z = 0, float xv = 0, float yv = 0, float zv = 0);
	void updatePosition(float timeStep);
	void resetForce();
	void bounceOffWalls(float left, float top, float right, float bottom, float damping = .3);// just make resolution?
	void addDampingForce(float damping = .01);
	void draw();
};
