#include "BinnedParticle.h"

#include "ofGraphics.h"

BinnedParticle::BinnedParticle(float x, float y, float z, float xv, float yv, float zv) :
	x(x), y(y), z(z),
	xv(xv), yv(yv), zv(zv) {
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
		x = right;
		xv *= -1;
		collision = true;
	} else if (x < left){
		x = left;
		xv *= -1;
		collision = true;
	}

	if (y > bottom){
		y = bottom;
		yv *= -1;
		collision = true;
	} else if (y < top){
		y = top;
		yv *= -1;
		collision = true;
	}

    //Change to depth??
    if (z > bottom){
        z = bottom;
        zv *= -1;
        collision = true;
    } else if (z < top){
        z = top;
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
	glVertex3f(x, y, z);
}
