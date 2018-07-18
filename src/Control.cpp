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
    
    frame test2;
    test2.frameType = 1;
    test2.totalTime = 10000;
    
    frames.push_back( test2 );
    
    //This is an edit tool to load in models and convert to my file format
    PlyRW plyR;
    frames.push_back( plyR.read("DecimatedJug.ply", kBinnedParticles) );
    

    
//    //just testing flow so far
//    loadFrame();
    

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
    
    latestFrame = &frames.back();
    
    cout << latestFrame->frameType << endl;
    
    if(!latestFrame->animating){
        startTime = ofGetElapsedTimeMillis();
        latestFrame->animating = true;
        loadFrame();
    }else{
        if(ofGetElapsedTimeMillis() - startTime > latestFrame->totalTime){
            latestFrame->animating = false;
            frame temp = frames.back();
            frames.push_front(temp);
            frames.pop_back();
        }
    }
    
    
    
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
        
        switch(latestFrame->frameType){
            case 1 : { // Flocking
                vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);

                for(int i = 0; i < neighbors.size(); i++){
                    cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
                }
                particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
                particleSystem.addAttractionForce(cur, particleNeighborhood+30, .3);
                
                break;
            }
            case 2 : { // Mesh and Colors to
                
                particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
                
                break;
            }
            default :
                break;

        }
        
        
        cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
        cur.addDampingForce();
    }
    
//    if(latestFrame->frameType == 1){
//            particleSystem.addAttractionForce(cubeResolution , cubeResolution , cubeResolution , cubeResolution * 100, centerAttraction);
//    }

    
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
    
    //Maybe better instead of types that you set in the struct
    //have a type be determined based on what data is available...
    
    //so frameType could just be a state i.e. flocking.
    //or should flocking be a holding pattern... i.e. emergence is what results after the moment in history?
    
    //Initialize a frame struct to have predicatble values...
    //Then check if those values have been filled with content.
    //If yes then push a type into a vector of ints.
    //call switch as many times as you have type...
    
    //OR could just do massive if else statements.... hmmmmmm
    
    switch(latestFrame->frameType){
        case 1 : { // Flocking

            break;
        }
        case 2 : { // Mesh and Colors
            
            if(latestFrame->points.size() > 0){
                
                //change this to activate the whole particle system
                for(int i = 0; i < latestFrame->points.size(); i++){
                    particleSystem[i].setTarget(latestFrame->points[i].x+cubeResolution/2, latestFrame->points[i].y+cubeResolution/2, latestFrame->points[i].z+cubeResolution/2);
                    if(latestFrame->pointColors.size() > 0){
                        particleSystem[i].targetColor = latestFrame->pointColors[i];
                    }
                }
            }
            break;
        }
        default : {
            //error handling if an incomplete json file ends up in data
            cout << "Unrecognized Frame Type" << endl;
            //Dispose of frame and continue with deque in next call to update
            frames.pop_back();
            break;
        }
            
    }
    
}


void Control::drawStats(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
    ofDrawBitmapString(ofToString((int) (ofGetElapsedTimeMillis() - startTime)) + " millis",32,72);
}



