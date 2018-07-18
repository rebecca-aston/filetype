//
//  Control.cpp
//  artifactlabEMMERSION
//
//  Created by Rebecca Aston on 7/17/18.
//

#include "Control.h"

Control::Control(){
    
}


void Control::setup(){
    // this number describes how many bins are used
    // on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
    // if this number is too high, binning is not effective
    // because the screen is not subdivided enough. if
    // it's too low, the bins take up so much memory as to
    // become inefficient.
    int binPower = 6;
    cubeResolution = 2000;
    particleSystem.setup(cubeResolution, cubeResolution, binPower);
    

    kBinnedParticles = 2000;
    for(int i = 0; i < kBinnedParticles; i++) {
        float x = ofRandom(0, cubeResolution) ;
        float y = ofRandom(0, cubeResolution) ;
        float z = ofRandom(0, cubeResolution) ;
        BinnedParticle particle(x, y, z, 0, 0, 0);
        particleSystem.add(particle);
    }
    
    
    //This is an edit tool to load in models and convert to my file format
    PlyRW plyR;
    frames.push_back( plyR.read("DecimatedJug.ply", kBinnedParticles) );
    
    //just testing flow so far
    loadFrame();
    

    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 64;
    particleRepulsion = .5;
    particleCohesion = .3;
    centerAttraction = .01;
    drawBalls = true;

    particleAlign = 0;
}

void Control::update(){
    
    //Few ways to "navigate" history, sequentially, i.e. the date timeline, geographically, speculatively
    //(i.e. word/sequence, algorithmically),
    
    //probably better to have one set of frames
    //and then pointers to the frames that are stored in deques
    //that way would only have to do sort once for history timeline
    //once for geographic etc etc
    //dynamic sorting would be the only "live" sorting
    
    //Manage deque size... potentially
    //look at the buffer logic from earlier class
    //pop_front if longer than certain amount of time....
    //And or just use the pop_front to move it around in time loop
    
    //Keep checking to see if the busy bool is true
    //if false push_back another frame onto the deque
    //get the last on deque to ->
//    loadFrame();
    
    particleSystem.setTimeStep(timeStep);
    
    // do this once per frame
    particleSystem.setupForces();
    
}

void Control::draw(){
    
    //Have to do this update logic here to see force lines drawn
    
    for(int i = 0; i < particleSystem.size(); i++) {
        BinnedParticle& cur = particleSystem[i];

        //        write better flow field
        //        particleSystem.flowField(cur.x, cur.y, cur.z, particleNeighborhood, particleRepulsion);
        
        //If i pulled this out of there entirely would have to be wary of the radius for the neighboorhood
//        vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);
//
//        for(int i = 0; i < neighbors.size(); i++){
//            cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
//        }
        
        
        //need to think about the manager..
        //how do we communicate that something is ready to move on?
        //is it just by time???
        

        if(cur.target){
//            particleSystem.addAttractionForce(cur.xt,cur.yt,cur.zt, 10000, 1);
            particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
        }else{
            particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
            particleSystem.addAttractionForce(cur, particleNeighborhood+30, .3);
        }
        
        cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
        cur.addDampingForce();
    }
    
    
//    particleSystem.addAttractionForce(cubeResolution / 2, cubeResolution / 2, cubeResolution / 2, cubeResolution * 100, centerAttraction);
    
    particleSystem.update(ofGetLastFrameTime());
    
    particleSystem.draw();
}

void Control::loadFrame(){
    
    //only replace or update new values that are given by a frame
    //so theoretically history could "overlay" itself
    //would result in synaesthetic fragments
    //but also allow for authorial intent to have a "clean" start whenever I want where everything changes over
    //it would act like those moments where more is know about a specific event
    //little snapshots into time
    
    //Can just use particleSystem[i].... not sure how but hey...
    
    Frame lastFrame = frames[frames.size()-1];
    
    if(lastFrame.getPoints().size() > 0){
        vector< ofVec3f > points = lastFrame.getPoints();
        vector< ofColor > colors = lastFrame.getPointColors();
        
        for(int i = 0; i < points.size(); i++){
            particleSystem.getParticles()[i]->setTarget(points[i].x+cubeResolution/2, points[i].y+cubeResolution/2, points[i].z+cubeResolution/2);
            if(colors.size() > 0){
                particleSystem.getParticles()[i]->targetColor = colors[i];
            }
        }
    }
    
}


void Control::drawStats(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
}



