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
    cout << xBins << endl;
    cout << yBins << endl;
    cout << xBins * yBins << endl;
	bins.resize(xBins * yBins * zBins);
}

void BinnedParticleSystem::setTimeStep(float timeStep) {
	this->timeStep = timeStep;
}

void BinnedParticleSystem::add(BinnedParticle particle) {
	particles.push_back(particle);
}

unsigned BinnedParticleSystem::size() const {
	return particles.size();
}

BinnedParticle& BinnedParticleSystem::operator[](unsigned i) { // still not sure what this is
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

    //All similar to the get region function, not sure why he crammed it all in here.
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

//ALIGNMENT
//OR
//STEERING

//steering force = desired velocity - current velocity


//Could do a Desired velocity for every particle in a bin / region.
//Thinking about the flow field implementation

//optimization
//probably better to just do the one loop of applying all forces/desires/behavior to each particle
//This is the loop that runs over all particles and was running a addSeparationForce
//Instead do the applyALL and one of those calls would be an apply force....??
//hmmm

//This seriously slows things down
//How to add alignment????
void BinnedParticleSystem::align(BinnedParticle& particle, float radius) {
    
    vector<BinnedParticle*> neighbors = getNeighbors(particle,radius);
    
    float nx, ny, nz;
    for(int i = 0; i < neighbors.size(); i++){
        nx += neighbors[i]->xv/timeStep;
        ny += neighbors[i]->yv/timeStep;
        nz += neighbors[i]->zv/timeStep;
    }
    
    particle.xf += (nx*0.001);
    particle.yf += (ny*0.001);
    particle.zf += (nz*0.001);
    
//    cout << "ALIGN: " ;
//    cout << particle.xv << endl;
    
}

//Aha stupidly named function
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

void BinnedParticleSystem::addRepulsionForce(const BinnedParticle& particle, float radius, float scale) {
	addRepulsionForce(particle.x, particle.y, particle.z, radius, scale);
}

void BinnedParticleSystem::addRepulsionForce(float x, float y, float z, float radius, float scale) {
	addForce(x, y, z, radius, scale);
}

void BinnedParticleSystem::addAttractionForce(const BinnedParticle& particle, float radius, float scale) {
	addAttractionForce(particle.x, particle.y, particle.z, radius, scale);
}

void BinnedParticleSystem::addAttractionForce(float x, float y, float z, float radius, float scale) {
	addForce(x, y, z, radius, -scale);
}

void BinnedParticleSystem::addForce(const BinnedParticle& particle, float radius, float scale) {
	addForce(particle.x, particle.y, particle.z, radius, -scale); 
}

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
    
    //All similar to the get region function, not sure why he crammed it all in here.
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
    
    
	float xd, yd, zd, length, maxrsq;
	#ifdef USE_INVSQRT
	float xhalf;
	int lengthi;
	#else
	float effect;
	#endif
    
    
	maxrsq = radius * radius;
    for(int z = minZBin; z < maxZBin; z++) {
        for(int y = minYBin; y < maxYBin; y++) { // ah so based off the radius has created a way to check the cols/rows next to where any potential center point of a particle could be.
            for(int x = minXBin; x < maxXBin; x++) {
                vector<BinnedParticle*>& curBin = bins[(z * yBins * xBins) + (y * xBins + x)];
                int n = curBin.size();
                for(int i = 0; i < n; i++) {
                    BinnedParticle& curBinnedParticle = *(curBin[i]);
                    xd = curBinnedParticle.x - targetX;
                    yd = curBinnedParticle.y - targetY;
                    zd = curBinnedParticle.z - targetZ;
                    length = xd * xd + yd * yd + zd * zd;
                    if(length > 0 && length < maxrsq) {// prevents them from bumping into each other
                        #ifdef DRAW_FORCES
                            glVertex3f(targetX, targetY, targetZ);
                            glVertex3f(curBinnedParticle.x, curBinnedParticle.y, curBinnedParticle.z);
                        #endif
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
            }
        }
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
	glBegin(GL_POINTS);
	for(int i = 0; i < n; i++)
		particles[i].draw();
	glEnd();
}

int BinnedParticleSystem::getWidth() const {
	return width;
}

int BinnedParticleSystem::getHeight() const {
	return height;
}
