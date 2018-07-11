#pragma once
#include "ofMain.h"

class Vert{
    
public:
    Vert(ofVec3f pos,int i,ofColor _color);//,vector<shared_ptr<Vert>> _vertices)
    
    void updateVertex(ofVec3f attractor,vector<Vert *> *_vertices, float SIZE,float DEPTH,float SPEED,float GRAVITY,float SIDEWAYS,float CHECKDIST);
    
//    ofVec3f separation(vector<shared_ptr<Vert>> *_vertices);
//    ofVec3f cohesion(vector<shared_ptr<Vert>> *_vertices);
//    ofVec3f align(vector<shared_ptr<Vert>> *_vertices);
    
    ofVec3f separation(vector<Vert*> *_vertices);
    ofVec3f cohesion(vector<Vert*> *_vertices);
    ofVec3f align(vector<Vert*> *_vertices);
    
//    vector<shared_ptr<Vert>> vertices;
    ofVec3f initPosition;
    ofVec3f position;
    ofVec3f velocity;
    int index, size;
    float distToAttractor, scale, distToCheck;
    ofColor color;

    ofIcoSpherePrimitive sphere;
    
};

