#include "BinnedParticleSystem.h"

BinnedParticleSystem::BinnedParticleSystem() :
	timeStep(100) {
}

void BinnedParticleSystem::setup(int width, int height, int k) {
	this->width = width;
	this->height = height;
	this->k = k;
	binSize = 1 << k; // bitwise operator, based on comment in ofApp.h and https://stackoverflow.com/questions/18215681/what-is-1-0 is equivalent of saying 1*pow(2,k)
	xBins = (int) ceilf((float) width / (float) binSize);
	yBins = (int) ceilf((float) height / (float) binSize);
    zBins = (int) ceilf((float) height / (float) binSize); //add depth??
//    cout << xBins << endl;
//    cout << yBins << endl;
//    cout << xBins * yBins << endl;
	bins.resize(xBins * yBins * zBins);
}

void BinnedParticleSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void BinnedParticleSystem::removeAtIndex(int i) {
    //Didn't use at index in the end
    
    particles.pop_back();

}

void BinnedParticleSystem::popFront(int amt) { 
    for(int i = 0;i < amt; i++){
        particles.pop_front();
    }
}

void BinnedParticleSystem::add(BinnedParticle particle) {
	particles.push_front(particle);
}

void BinnedParticleSystem::clear(){
    particles.clear();
}

void BinnedParticleSystem::setOwner(string s){
    particleOwner = s;
}

string BinnedParticleSystem::getOwner(){
    return particleOwner;
}

vector<BinnedParticle> BinnedParticleSystem::getParticles(){
    
    vector<BinnedParticle> temp;
    for(int i = 0; i < particles.size(); i++) {
        temp.push_back(particles[i]);
    }
    return temp;
    
}

unsigned BinnedParticleSystem::size() const {
	return particles.size();
}

BinnedParticle& BinnedParticleSystem::operator[](unsigned i) { // cool, this makes particles vector public to Psystem
	return particles[i];
}

vector<BinnedParticle*> BinnedParticleSystem::getNeighbors(BinnedParticle& particle, float radius) {
    return getNeighbors(particle.x, particle.y, particle.z, radius);
}

vector<BinnedParticle*> BinnedParticleSystem::getNeighbors(float x, float y, float z, float radius) {
    vector<BinnedParticle*> region = getRegion(
        (int) (x - radius),
        (int) (x + radius),
        (int) (y - radius),
        (int) (y + radius),
        (int) (z - radius),
        (int) (z + radius));
    
    vector<BinnedParticle*> neighbors;
    int n = region.size();
    
    float xd, yd, zd, rsq, maxrsq;
    maxrsq = radius * radius;
    
    for(int i = 0; i < n; i++) {
        BinnedParticle& cur = *region[i];
        xd = cur.x - x;
        yd = cur.y - y;
        zd = cur.z - z;
        rsq = xd * xd + yd * yd + zd * zd;
        if(rsq < maxrsq)
            neighbors.push_back(region[i]);
    }
    
    return neighbors;
}

vector<BinnedParticle*> BinnedParticleSystem::getRegion(unsigned minX, unsigned maxX,unsigned minY, unsigned maxY,unsigned minZ, unsigned maxZ) {
    vector<BinnedParticle*> region;
    back_insert_iterator< vector<BinnedParticle*> > back = back_inserter(region);// this allows copy (below) to insert elements into back of container (region) rather than over-writing whole vector

    unsigned minXBin = ((unsigned) minX) >> k;
    unsigned minYBin = ((unsigned) minY) >> k;
    unsigned minZBin = ((unsigned) minZ) >> k;
    unsigned maxXBin = ((unsigned) maxX) >> k;
    unsigned maxYBin = ((unsigned) maxY) >> k;
    unsigned maxZBin = ((unsigned) maxZ) >> k;
    
    maxXBin++;
    maxYBin++;
    maxZBin++;
    
    if(maxXBin > xBins)
        maxXBin = xBins;
    if(maxYBin > yBins)
        maxYBin = yBins;
    if(maxZBin > zBins)
        maxZBin = zBins;

    for(int z = minZBin; z < maxZBin; z++) {
        for(int y = minYBin; y < maxYBin; y++) { // ah so based off the radius has created a way to check the cols/rows next to where any potential center point of a particle could be.
            for(int x = minXBin; x < maxXBin; x++) {
                vector<BinnedParticle*>& curBin = bins[(z * yBins * xBins) + (y * xBins + x)];
                copy(curBin.begin(), curBin.end(), back); // copys vector into region vector
            }
        }
    }
    
    return region;
}

//This is more like setupBins
void BinnedParticleSystem::setupForces() {
	int n = bins.size();
	for(int i = 0; i < n; i++) {
		bins[i].clear();
	}
	n = particles.size();
	unsigned xBin, yBin, zBin, bin;
	for(int i = 0; i < n; i++) {
		BinnedParticle& cur = particles[i];
		cur.resetForce();
		xBin = ((unsigned) cur.x) >> k;
		yBin = ((unsigned) cur.y) >> k;
        zBin = ((unsigned) cur.z) >> k;
		bin = (zBin * yBins * xBins) + (yBin * xBins + xBin);
		if(xBin < xBins && yBin < yBins && zBin < zBins)
			bins[bin].push_back(&cur);
	}
}

//Particle specific
void BinnedParticleSystem::addRepulsionForce(const BinnedParticle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, particle.z, radius, scale);
}

//Global force
void BinnedParticleSystem::addRepulsionForce(float x, float y, float z, float radius, float scale) {
	addForce(x, y, z, radius, scale);
}

//Particle specific
void BinnedParticleSystem::addAttractionForce(const BinnedParticle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, particle.z, radius, scale);
}

//Global force
void BinnedParticleSystem::addAttractionForce(float x, float y, float z, float radius, float scale) {
	addForce(x, y, z, radius, -scale);
}

void BinnedParticleSystem::addForce(const BinnedParticle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, particle.z, radius, -scale); 
}

//I split Kyle's original function into two so that I could just use the force without the binning if need be
void BinnedParticleSystem::addForce(float targetX, float targetY, float targetZ, float radius, float scale) {
	float minX = targetX - radius;
	float minY = targetY - radius;
    float minZ = targetZ - radius;
	float maxX = targetX + radius;
	float maxY = targetY + radius;
    float maxZ = targetZ + radius;
	if(minX < 0)
		minX = 0;
	if(minY < 0)
		minY = 0;
    if(minZ < 0)
        minZ = 0;
    
	unsigned minXBin = ((unsigned) minX) >> k;
	unsigned minYBin = ((unsigned) minY) >> k;
    unsigned minZBin = ((unsigned) minZ) >> k;
	unsigned maxXBin = ((unsigned) maxX) >> k;
	unsigned maxYBin = ((unsigned) maxY) >> k;
    unsigned maxZBin = ((unsigned) maxZ) >> k;

	maxXBin++;
	maxYBin++;
    maxZBin++;
	if(maxXBin > xBins)
		maxXBin = xBins;
	if(maxYBin > yBins)
		maxYBin = yBins;
    if(maxZBin > zBins)
        maxZBin = zBins;
    
    for(int z = minZBin; z < maxZBin; z++) {
        for(int y = minYBin; y < maxYBin; y++) { // ah so based off the radius has created a way to check the cols/rows next to where any potential center point of a particle could be.
            for(int x = minXBin; x < maxXBin; x++) {
                vector<BinnedParticle*>& curBin = bins[(z * yBins * xBins) + (y * xBins + x)];
                int n = curBin.size();
                for(int i = 0; i < n; i++) {
                    

                    BinnedParticle& curBinnedParticle = *(curBin[i]);
                    
                    force(curBinnedParticle,targetX,targetY,targetZ, radius,  scale);
                    
                }
            }
        }
    }
}

//I split Kyle's original function into two so that I could just use the force without the binning if need be
void BinnedParticleSystem::force(BinnedParticle& curBinnedParticle,float targetX,float targetY,float targetZ, float radius, float scale){
    
    float xd, yd, zd, length;
    #ifdef USE_INVSQRT
        float xhalf;
        int lengthi;
    #else
        float effect;
    #endif
    
        float maxrsq;
    
        maxrsq = radius * radius;
    
        xd = curBinnedParticle.x - targetX;
        yd = curBinnedParticle.y - targetY;
        zd = curBinnedParticle.z - targetZ;
    
        length = xd * xd + yd * yd + zd * zd;
    
        if(curBinnedParticle.target){
            curBinnedParticle.targetDist = length;
        }
    
        if(length > 0 && length < maxrsq) {// prevents them from bumping into each other

    //        ofSetColor(255,100);
    //        glBegin(GL_LINE_STRIP);
    //        glVertex3f(targetX, targetY, targetZ);
    //        glVertex3f(curBinnedParticle.x, curBinnedParticle.y, curBinnedParticle.z);
    //        glEnd();
            
    #ifdef USE_INVSQRT // optimization for the sqrt efficiency cost
            xhalf = 0.5f * length;
            lengthi = *(int*) &length;
            lengthi = 0x5f3759df - (lengthi >> 1);
            length = *(float*) &lengthi;
            length *= 1.5f - xhalf * length * length;
            xd *= length;
            yd *= length;
            zd *= length;
            length *= radius;
            length = 1 / length;
            length = (1 - length);
    #ifdef USE_SMOOTH_FORCES
            length = smoothForce(length);
    #endif
            length *= scale;
            xd *= length;
            yd *= length;
            zd *= length;
            curBinnedParticle.xf += xd;
            curBinnedParticle.yf += yd;
            curBinnedParticle.zf += zd;
    #else
            length = sqrtf(length);
    #ifdef USE_SMOOTH_FORCES
            length = smoothForce(length);
    #endif
            xd /= length;
            yd /= length;
            zd /= length;
            effect = (1 - (length / radius)) * scale;
            curBinnedParticle.xf += xd * effect;
            curBinnedParticle.yf += yd * effect;
            curBinnedParticle.zf += zd * effect;
    #endif
    }
    
}

void BinnedParticleSystem::update(float lastTimeStep) {
	int n = particles.size();
	float curTimeStep = lastTimeStep * timeStep;
	for(int i = 0; i < n; i++) {
		particles[i].updatePosition(curTimeStep);
	}
}

void BinnedParticleSystem::draw() {
	int n = particles.size();
    glPointSize(5);
//    glBegin(GL_POINTS);
    for(int i = 0; i < n; i++){
//            glBegin(GL_TRIANGLES);
//        for(int j = i; j < i+3; j++){
//            particles[j].draw();
        particles[i].draw();
//        }
//            glEnd();
    }
//    glEnd();
}

int BinnedParticleSystem::getWidth() const {
	return width;
}

int BinnedParticleSystem::getHeight() const {
	return height;
}
