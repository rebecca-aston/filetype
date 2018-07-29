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
    
    
    //Load in all json frames from data files into the frames deque
    
    frame test2;
    test2.frameType = 1;
    test2.totalTime = 10000;
    
    frames.push_back( test2 );
    
    //This is an edit tool to load in models and convert to my file format
    PlyRW plyR;
    frames.push_back( plyR.read("DecimatedJug.ply", kBinnedParticles) );
    
    
//    Then write some proper "sort" algorithms that would take the frames and push into deque of pointers
    for(int i = 0; i < frames.size(); i++){
        sequence.push_back(&frames[i]);
    }
    
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
    
    if(!currentFrame.animating){
        startTime = ofGetElapsedTimeMillis();
        currentFrame.animating = true;
        loadFrame();
    }else{
        if(ofGetElapsedTimeMillis() - startTime > currentFrame.totalTime){
            currentFrame.animating = false;
            frame * temp = sequence.back();
            sequence.push_front(temp);
            sequence.pop_back();
        }
    }
    

   //particle system update logic
    particleSystem.setTimeStep(timeStep);
    
    // do this once per frame
    particleSystem.setupForces();
    
    for(int i = 0; i < particleSystem.size(); i++) {
        BinnedParticle& cur = particleSystem[i];
        
        //        write better flow field
        //        particleSystem.flowField(cur.x, cur.y, cur.z, particleNeighborhood, particleRepulsion);
        
        switch(currentFrame.frameType){
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
    
    
}

void Control::loadFrame(){
    
    //Maybe better instead of types that you set in the struct
    //have a type be determined based on what data is available...
    
    //so frameType could just be a state i.e. flocking.
    //or should flocking be a holding pattern... i.e. emergence is what results after the moment in history?
    
    //Initialize a frame struct to have predicatble values...
    //Then check if those values have been filled with content.
    //If yes then push a type into a vector of ints.
    //call switch as many times as you have type...
    
    //OR could just do massive if else statements.... hmmmmmm
    
    currentFrame.frameType = sequence.back()->frameType;
    currentFrame.totalTime = sequence.back()->totalTime;
    
    switch(currentFrame.frameType){
        case 1 : { // Flocking
            
            break;
        }
        case 2 : { // Mesh and Colors
            currentFrame.points = sequence.back()->points;
            currentFrame.pointColors = sequence.back()->pointColors;
            
            if(currentFrame.points.size() > 0){
                
                //change this to activate the whole particle system
//                for(int i = 0; i < currentFrame.points.size(); i++){
//                    particleSystem[i].setTarget(currentFrame.points[i].x+cubeResolution/2, currentFrame.points[i].y+cubeResolution/2, currentFrame.points[i].z+cubeResolution/2);
//                    if(currentFrame.pointColors.size() > 0){
//                        particleSystem[i].targetColor = currentFrame.pointColors[i];
//                    }
//                }
                
                //load Frame only gets called once on transition
                //convert one point to be the origin point and figure out distance in relation to that.
                //Find the direction and the magnitude from the "origin point"/vertex
                //Store that mag and direction for the two other points on the triangle
                //then draw a vertex at that distance and direction from the "origin" point of the particle
                
                int count = 0;
                
                //triangulation
                for(int i = 0; i < currentFrame.points.size(); i+=3){
                    
                    //The first point of each triangle is the "origin"
//                    particleSystem[i].setTarget(currentFrame.points[i].x+cubeResolution/2, currentFrame.points[i].y+cubeResolution/2, currentFrame.points[i].z+cubeResolution/2);
                    
                    //do I really need to break it down to mag and direction to figure this out?
                    //can I not just store the length? no....
                    //how to get the vector from origin in direction X mag?
                    
                    //where p1 and p2 are an ofVec3f which is the directional vector of:
                    //point 0 or ORIGIN minus (as in vector subtraction) the original point 1 and 2
                    //This is then just added(?) to the point 0 only in the draw of the particle.
//                    particleSystem[i].p1
//                     particleSystem[i].p2
                    
              
                    
                    if(count < 2000){
                        ofVec3f diff1 = currentFrame.points[i] - currentFrame.points[i+1];
                        ofVec3f diff2 = currentFrame.points[i] - currentFrame.points[i+2];
                        
                        particleSystem[count].setTarget(currentFrame.points[i].x+cubeResolution/2, currentFrame.points[i].y+cubeResolution/2, currentFrame.points[i].z+cubeResolution/2);
                        
                        particleSystem[count].p1 = diff1;
                        particleSystem[count].p2 = diff2;
                        
                        
                        if(currentFrame.pointColors.size() > 0){
                            particleSystem[count].targetColor = currentFrame.pointColors[i];
                            particleSystem[count].p1Color  = currentFrame.pointColors[i+1];
                            particleSystem[count].p1Color  = currentFrame.pointColors[i+2];
                        }
                    }

                    count ++;
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

//Keep only display in here so that can swivel around a static state
void Control::draw(){
    particleSystem.draw();
}

void Control::drawStats(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
    ofDrawBitmapString(ofToString((int) (ofGetElapsedTimeMillis() - startTime)) + " millis",32,72);
}



