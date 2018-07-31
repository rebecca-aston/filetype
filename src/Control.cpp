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
    cubeResolution = 1000;
    particleSystem.setup(cubeResolution, cubeResolution, binPower);
    

    kBinnedParticles = 50000;//25000 is enough to do all triangles
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
    test2.totalTime = 30000;

    
    frames.push_back( test2 );
    
    //This is an edit tool to load in models and convert to my file format
    PlyRW plyR;
    
    //    PlyRW plyR;
    
 
    

    
    
    frame test1;
    test1.frameType = 2;
    test1.totalTime = 20000;
    
    test1.mesh = plyR.read("jug6857.ply", kBinnedParticles);

    
    for(int i = 0; i < test1.mesh.getVertices().size();i++){
        test1.mesh.setVertex(i, test1.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution*.8,cubeResolution/2));
    }
    
    
    frames.push_back( test1 );
    
    
  
    
    
    
    frame test5;
    test5.frameType = 2;
    test5.totalTime = 20000;
    
    //    test1.mesh = plyR.read("jug6857.ply", kBinnedParticles);
    test5.mesh = plyR.read("LoungeCouch.ply", kBinnedParticles);
    
    for(int i = 0; i < test5.mesh.getVertices().size();i++){
        test5.mesh.setVertex(i, test5.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution*.8,cubeResolution/2));
    }
    
    
    frames.push_back( test5 );
    

    frame test4;
    test4.frameType = 2;
    test4.totalTime = 20000;
    
    //    test1.mesh = plyR.read("jug6857.ply", kBinnedParticles);
    test4.mesh = plyR.read("LungeLeftWallDecimated0.5.ply", kBinnedParticles);
    
    for(int i = 0; i < test4.mesh.getVertices().size();i++){
        test4.mesh.setVertex(i, test4.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution*.8,cubeResolution/2));
    }
    
    
    frames.push_back( test4 );


    
    frame test3;
    test3.frameType = 2;
    test3.totalTime = 20000;
    //    test3.renderMesh = true;
    
    test3.mesh = plyR.read("contextDecimated0.5.ply", kBinnedParticles);
    
    for(int i = 0; i < test3.mesh.getVertices().size();i++){
        test3.mesh.setVertex(i, test3.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution*.8,cubeResolution/2));
    }
    
    
    frames.push_back( test3 );
    
    
    

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
    

        //        write better flow field
        //        particleSystem.flowField(cur.x, cur.y, cur.z, particleNeighborhood, particleRepulsion);
        
        switch(currentFrame.frameType){
            case 1 : { // Flocking

                
                for(int i = 0; i < particleSystem.size(); i++) {
                    BinnedParticle& cur = particleSystem[i];
                    //                vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);
                    //
                    //                for(int i = 0; i < neighbors.size(); i++){
                    //                    cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
                    //                }
                    //                particleSystem.addRepulsionForce(cur, 300, particleRepulsion);
                    //                particleSystem.addAttractionForce(cur, particleNeighborhood+30, .3);


//                    cur.addDampingForce();
                    cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
                }
                
                
                for(int i = currentFrame.leader - 20; i < currentFrame.leader + 20; i++){
                    BinnedParticle& cur = particleSystem[i];
                    vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);
                    
                    for(int i = 0; i < neighbors.size(); i++){
                        cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
                    }
                    
                    particleSystem.addRepulsionForce(cur, 70, .1);
                    particleSystem.addAttractionForce(cur, 50, .3);
 
   
                }
                
                
                particleSystem.addAttractionForce(particleSystem[currentFrame.leader], particleNeighborhood+30, .3);
            
                
                break;
            }
            case 2 : { // Mesh and Colors to
                
            
                for(int i = 0; i < particleSystem.size(); i++) {
                    BinnedParticle& cur = particleSystem[i];
                    
                    particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
                
                    cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
                    cur.addDampingForce();
                }

                break;
            }
            case 3 : { // Mesh and Colors to
                
                if(currentFrame.points.size() > 0){
                int count = 0;
                
                //triangulation
                for(int i = 0; i < currentFrame.points.size(); i+=3){
                    
                    if(count < kBinnedParticles){
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
//                particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
                
                
                
                break;
            }
            default :
                break;
                
        }
        
    
    
    particleSystem.update(ofGetLastFrameTime());
    
    
}

void Control::loadFrame(){
    
    currentFrame.frameType = sequence.back()->frameType;
    currentFrame.totalTime = sequence.back()->totalTime;
    
    switch(currentFrame.frameType){
        case 1 : { // Flocking
            if(sequence.back()->leader != -1){
                currentFrame.leader = sequence.back()->leader;
            }else{
                currentFrame.leader = ofRandom(200,kBinnedParticles-200);
            }
            
            
            break;
        }
        case 2 : { // Mesh and Colors
//            currentFrame.points = sequence.back()->points;
//            currentFrame.pointColors = sequence.back()->pointColors;
            
            currentFrame.renderMesh = sequence.back()->renderMesh;
            
            
//            if(sequence.back()->mesh.getVertices().size() > 0 && sequence.back()->particles.size() == 0){
            
                
//                for (int i = 0; i < sequence.back()->mesh.getIndices().size()-3; i++){
//
//                        float x = ofRandom(0, cubeResolution) ;
//                        float y = ofRandom(0, cubeResolution) ;
//                        float z = ofRandom(0, cubeResolution) ;
//                        BinnedParticle particle(x, y, z, 0, 0, 0);
//
//
//
//                        ofVec3f t = currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i));
//                        particle.setTarget(t.x,t.y,t.z);
//
//                        ofVec3f diff1 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+1));
//                        ofVec3f diff2 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+2));
//
//                        particle.p1 = diff1;
//                        particle.p2 = diff2;
//
//                        particle.targetColor = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i));
//                        particle.p1Color = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+1));
//                        particle.p2Color  = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+2));
//
//                        sequence.back()->particles.push_back(particle);
//
//                }
//
//
//            }else{
//
//            }
            
            
            
            for(int i = 0; i < particleSystem.size(); i++) {
                
                particleSystem[i].setTarget(particleSystem[i].x, particleSystem[i].y, 0);
                
            }
            
            if(sequence.back()->mesh.getVertices().size() > 0){
                currentFrame.mesh = sequence.back()->mesh;
            }
            
            if(currentFrame.mesh.getVertices().size() > 0){
                int count = 0;
                
                //triangulation

//                cout << currentFrame.mesh.getIndices().size() << endl;
//                cout << currentFrame.mesh.getVertices().size() << endl;
                    for (int i = 0; i < currentFrame.mesh.getIndices().size()-3; i++){
                        
                        if(count < kBinnedParticles){ //need to actually do the triangulation thing....
                            
                            ofVec3f t = currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i));
                            particleSystem[count].setTarget(t.x,t.y,t.z);
                            
                            ofVec3f diff1 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+1));
                            ofVec3f diff2 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+2));
                            
                            particleSystem[count].p1 = diff1;
                            particleSystem[count].p2 = diff2;
                            
                            particleSystem[count].targetColor = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i));
                            particleSystem[count].p1Color = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+1));
                             particleSystem[count].p2Color  = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+2));
     
                            count ++;
                        }
                    }
                    
//                cout << count << endl;
                
            }
            
            
            break;
        }
        case 3 : { // Mesh and Colors
            currentFrame.mesh = sequence.back()->mesh;
            currentFrame.renderMesh = sequence.back()->renderMesh;
            
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
    

    if(currentFrame.mesh.getVertices().size() > 0 && currentFrame.renderMesh){
        currentFrame.mesh.drawWireframe();
    }
    
    
}

void Control::drawStats(){
    ofSetColor(255);
    ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
    ofDrawBitmapString(ofToString((int) (ofGetElapsedTimeMillis() - startTime)) + " millis",32,72);
}

void Control::exportPLY(){
    frame temp;
    PlyRW w;
    
    for(int i = 0; i < particleSystem.size(); i++){
        if(particleSystem[i].exp){ //uninitialized
            ofVec3f p = ofVec3f(particleSystem[i].x,particleSystem[i].y,particleSystem[i].z);
            ofVec3f p1 = ofVec3f(p.x + particleSystem[i].p1.x, p.y + particleSystem[i].p1.y, p.z + particleSystem[i].p1.z);
            ofVec3f p2 = ofVec3f(p.x + particleSystem[i].p2.x, p.y + particleSystem[i].p2.y, p.z + particleSystem[i].p2.z);
            if(p.distance(p1) < 30 && p.distance(p2) < 30){
                temp.points.push_back(p);
                temp.points.push_back(p1);
                temp.points.push_back(p2);
                temp.pointColors.push_back(particleSystem[i].targetColor);
                temp.pointColors.push_back(particleSystem[i].p1Color);
                temp.pointColors.push_back(particleSystem[i].p2Color);
            }
   
        }
    }

    w.write(temp,"pointCloud_"+ofGetTimestampString());
}

