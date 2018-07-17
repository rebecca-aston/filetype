#include "BinnedParticle.h"

#include "ofGraphics.h"

BinnedParticle::BinnedParticle(float x, float y, float z, float xv, float yv, float zv) :
	x(x), y(y), z(z),
	xv(xv), yv(yv), zv(zv) {
        color = ofColor(255);
}

void BinnedParticle::updatePosition(float timeStep) {
	// f = ma, m = 1, f = a, v = int(a)
	xv += xf * timeStep;
	yv += yf * timeStep;
    zv += zf * timeStep;
	x += xv * timeStep;
	y += yv * timeStep;
    z += zv * timeStep;
}

void BinnedParticle::resetForce() {
	xf = 0;
	yf = 0;
    zf = 0;
}

void BinnedParticle::bounceOffWalls(float left, float top, float right, float bottom, float damping) {
	bool collision = false;

	if (x > right){
        x = right;//-10;
		xv *= -1;
		collision = true;
	} else if (x < left){
        x = left;//+10;
		xv *= -1;
		collision = true;
	}

	if (y > bottom){
        y = bottom;//-10;
		yv *= -1;
		collision = true;
	} else if (y < top){
        y = top;//+10;
		yv *= -1;
		collision = true;
	}

    //Change to depth??
    if (z > bottom){
        z = bottom;//-10;
        zv *= -1;
        collision = true;
    } else if (z < top){
        z = top;//+10;
        zv *= -1;
        collision = true;
    }
    
	if (collision == true){
		xv *= damping;
		yv *= damping;
        zv *= damping;
	}
}

void BinnedParticle::addDampingForce(float damping) {
	xf -= xv * damping;
	yf -= yv * damping;
    zf -= zv * damping;
}

void BinnedParticle::draw() {
//    if(color != targetColor) lerp color...
    ofSetColor(color);
	glVertex3f(x, y, z);
}
