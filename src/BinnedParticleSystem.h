#pragma once

#include "ofMain.h"
#include "BinnedParticle.h"

#define DRAW_FORCES
#define USE_INVSQRT
#define USE_SMOOTH_FORCES

class BinnedParticleSystem {
protected:
	float timeStep;
	vector<BinnedParticle> particles; // a vector of particles
	vector< vector<BinnedParticle*> > bins; // vector of vectors of pointers to particles
	int width, height, k, xBins, yBins, zBins, binSize;

public:
	BinnedParticleSystem();

	void setup(int width, int height, int k);
	void setTimeStep(float timeStep);

	void add(BinnedParticle particle);
    vector<BinnedParticle*> getParticles();
	vector<BinnedParticle*> getNeighbors(BinnedParticle& particle, float radius);
	vector<BinnedParticle*> getNeighbors(float x, float y, float z, float radius);
	vector<BinnedParticle*> getRegion(unsigned minX, unsigned maxX, unsigned minY, unsigned maxY,unsigned minZ, unsigned maxZ);
	unsigned size() const;
	BinnedParticle& operator[](unsigned i); // not sure what this is?

    void flowField(float targetX, float targetY, float targetZ, float radius, float scale);

	void setupForces();
	void addRepulsionForce(const BinnedParticle& particle, float radius, float scale);
	void addRepulsionForce(float x, float y, float z, float radius, float scale);
	void addAttractionForce(const BinnedParticle& particle, float radius, float scale);
	void addAttractionForce(float x, float y, float z, float radius, float scale);
	void addForce(const BinnedParticle& particle, float radius, float scale);
	void addForce(float x, float y, float z, float radius, float scale);
    void force(BinnedParticle& curBinnedParticle,float targetX,float targetY,float targetZ,float radius, float scale);
	void update(float lastTimeStep);

	void draw();
	
	int getWidth() const;
	int getHeight() const;
};

inline float InvSqrt(float x){ // making it inline function improves efficiency at runtime
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}

inline float smoothForce(float x) {
	const static float sharpness = 1;
	return 1. / (1. + expf((x - .5) * sharpness * -12));
}
