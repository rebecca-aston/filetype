#include "BinnedParticle.h"

#include "ofGraphics.h"

BinnedParticle::BinnedParticle(float x, float y, float z, float xv, float yv, float zv, float l) :
	x(x), y(y), z(z),
	xv(xv), yv(yv), zv(zv), life(l) {
        color = ofColor(255);
        target = false;
        targetDist = 1000;
        triMode = true;
}

void BinnedParticle::updatePosition(float timeStep) {
	// f = ma, m = 1, f = a, v = int(a)
	xv += xf + xa * timeStep;
	yv += yf + ya * timeStep;
    zv += zf + za* timeStep;
	x += xv * timeStep;
	y += yv * timeStep;
    z += zv * timeStep;
    
    //needs work
    if(life < 100000 ) {
        
        life -= 0.2;

        targetColor = ofColor(targetColor.r,targetColor.g,targetColor.b,MAX(0,life));
    }
    
    if(target && targetDist < 10) {
        xv = 0;
        yv = 0;
        zv = 0;
        target = false;
    }
}

void BinnedParticle::resetForce() {
    xa = 0;
    ya = 0;
    za = 0;
	xf = 0;
	yf = 0;
    zf = 0;
}

//alternate bounds/action
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

    if (z > bottom){
        z = bottom;//-10;
        zv *= -1;
        
        collision = true;
        
    }
    else if (z < top){
        z = top;
        zv *= -1.5;
        collision = true;
    }
    
	if (collision == true){
		xv *= damping;
		yv *= damping;
        zv *= damping;
	}
}

void BinnedParticle::bounceOffFloor(float floor, float damping) {
    bool collision = false;

    //Change to depth??
    if (z < floor){
        z = floor;//-10;
        zv *= -1.5;
        
        collision = true;
        
    }
    
    if (collision == true){
        xv *= damping;
        yv *= damping;
        zv *= damping;
    }
}

//alternate bounds/action
void BinnedParticle::swirl() {
    xv += 0.001 * cos (TWO_PI * ofNoise(1.5 * x, 1.5 * y, 0.01 * z));
}

void BinnedParticle::addDampingForce(float damping) {
	xf -= xv * damping;
	yf -= yv * damping;
    zf -= zv * damping;
}

void BinnedParticle::align(float _xv, float _yv, float _zv) {
    xa += _xv * 0.001;
    ya += _yv * 0.001;
    za += _zv * 0.001;
}

void BinnedParticle::setTarget(float _x, float _y, float _z) {
    target = true;
    xt = _x;
    yt = _y;
    zt = _z;
}

void BinnedParticle::setLife(int l){
    
    life = l;

}

//Binned particle has one main node
//then it has two other nodes
//the first node of it's node array is the one that gets updated OR checked against
//the second two are just having the same velocity added to them. 

void BinnedParticle::draw() {
    
    //The legacy GL draw solution

    ofPushStyle();
        glBegin(GL_TRIANGLES);
            ofSetColor(targetColor);
            glVertex3f(x, y, z);
            glVertex3f(x-p1.x,y-p1.y,z-p1.z);
            glVertex3f(x-p2.x,y-p2.y,z-p2.z);
        glEnd();
    ofPopStyle();
    
}



