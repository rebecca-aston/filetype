#include "Vert.h"

// Constructor
Vert::Vert(ofVec3f pos, int i, ofColor _color){

//    vertices = _vertices;
    initPosition  = pos;
    position = initPosition;
    velocity = ofVec3f(0,0,0);
    index = i;
    color = _color;
    
//    size = ofRandom(2,4);
    size = 4;
    
//    cout << position << endl;
    sphere.set(size,3);
    for(int i = 0; i < sphere.getMesh().getVertices().size();i++){
        sphere.getMesh().addColor(_color);
    }
}

//--------------------------------------------------------------
void Vert::updateVertex(ofVec3f attractor, vector<Vert*>* _vertices, float SIZE,float PROBABILITY,float SPEED,float GRAVITY,float SIDEWAYS, float CHECKDIST){
    
    //Runs faster with a transform rather than setRadius
    
//    scale = SIZE;
//    sphere.setScale(scale);
    
    distToCheck = ofMap(CHECKDIST,-1,1,0,100);
    
    ofVec3f desired = attractor - position;
    distToAttractor = desired.lengthSquared();
    
    scale = MIN(-3,ofMap(distToAttractor,0,10000,4,3));
    sphere.setScale(scale);
    
//    sphere.setResolution(ofMap(distToAttractor/SIZE,0,5000,1,4));
    
    if(ofRandom(1)>PROBABILITY && SPEED > 0){
        desired.normalize();
        desired *= SPEED;
        velocity = desired - velocity;
    }
    
    velocity += separation(_vertices);
    velocity += cohesion(_vertices);
    velocity += align(_vertices);
    
    velocity = velocity.limit(1.5);

    position += velocity;
    
    sphere.setPosition(position);

}

ofVec3f Vert::separation(vector<Vert*>* _vertices){
    ofVec3f RES = ofVec3f(0,0,0);
    // query the entire flock
    for(int i = 0 ; i < _vertices->size();i++){

        float dist = position.distance(_vertices->at(i)->position);

        if(dist < distToCheck && dist > 0){//30//
            // it's close enough >> add a difference vector
            RES -= (_vertices->at(i)->position - position);
        }
    }
    
    RES *= 0.07;
    return RES;
}

ofVec3f Vert::cohesion(vector<Vert*> *_vertices){
    ofVec3f RES = ofVec3f(0,0,0);
    int numNeighbours = 0;
    
    // query the entire flock
    for(int i = 0 ; i < _vertices->size();i++){
        float dist = position.distance(_vertices->at(i)->position);

        if(dist < distToCheck+20 && dist > size+_vertices->at(i)->size){//50
            // its close enough >> add its position to the avarage
            RES += _vertices->at(i)->position;
            numNeighbours++;
        }

    }
    
    if(numNeighbours == 0) return ofVec3f(0,0,0);
    
    RES /= numNeighbours;
    RES -= position;
    RES *= 0.01;
    return RES;
}

ofVec3f Vert::align(vector<Vert*> *_vertices){
    ofVec3f RES = ofVec3f(0,0,0);
    int numNeighbours = 0;
    
    // query the entire flock
    for(int i = 0 ; i < _vertices->size();i++){

        float dist = position.distance(_vertices->at(i)->position);
        
        if(dist < distToCheck+10 && dist > size+_vertices->at(i)->size){//40 //
            // it's close enough >> add its velocity to the avarage
            RES += _vertices->at(i)->velocity;
            numNeighbours++;
        }
    }
    
    if(numNeighbours == 0) return ofVec3f(0,0,0);
    
    RES /= numNeighbours;
    RES *= 0.3;
    return RES;
}


