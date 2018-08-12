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
    
    read.readModel();
    
    //Just initialize the sequence to the first few
    for(int i = 0; i < read.frameVec.size(); i++){
        if(sequence.size() < 10){
            sequence.push_back(&read.frameVec[i]);
        }else{
            break;
        }
    }
    
    // this number describes how many bins are used
    // on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
    // if this number is too high, binning is not effective
    // because the screen is not subdivided enough. if
    // it's too low, the bins take up so much memory as to
    // become inefficient.
    int binPower = 6;
    cubeResolution = 1000;
    
    //The active particles
    particleSystem.setup(cubeResolution, cubeResolution, binPower);
    
    kBinnedParticles = 50000;//25000 is enough to do all triangles
    backBurnerSystem.setup(cubeResolution*1.5, cubeResolution*1.5, binPower);
    
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
    
    //Check animation timing to see if should load in new frame
    //Then loadFrame()
    
    //Rather than time based... should probably just be stay in the holding pattern
    //Until new data is added
    //But not sure....
    
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
    
    
    //Current setup
    //Main particle system that is the sort of "active" state
    //The new frame is loading in.
    //Then there is another "system" which is the scattered accumulation of particles
    //This is currently mostly a techincal consideration rather than a conceptual one
    //Have the secondary system only be activated by less computationally expensive actions
    // maybe re-think the setup?
    
    //ACTIVE
    particleSystem.setTimeStep(timeStep);
    //BACKGROUND
    backBurnerSystem.setTimeStep(timeStep);
    
    //ACTIVE
    particleSystem.setupForces();
    //BACKGROUND
    backBurnerSystem.setupForces();
    
        
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


//                  cur.addDampingForce();
                    cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
                }
                
                //Only apply flocking to a random selection of particles
                //Maybe a better way to do it, but keeps things lighter on the system while preserving some nice behavior
                for(int i = MAX(0,currentFrame.leader - 10); i < MIN(particleSystem.size(),currentFrame.leader + 10); i++){
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
            case 2 : { // Mesh and Colors in active particle sys
                
                for(int i = 0; i < particleSystem.size(); i++) {
                    BinnedParticle& cur = particleSystem[i];
                    
                    particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
                
                    cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
                    cur.addDampingForce();
                }

                break;
            }
            case 3 : { // Mesh draw
                
                break;
            }
            default :
                break;
                
        }
    
    
    
    //BACKGROUND
    
    //Maybe better to think of this background as the flow field / global forces (although ff might be too heavy)
    //Map data / trade routes getting thrown in as movement/flow vectors..
    
    //        write better flow field
    //        particleSystem.flowField(cur.x, cur.y, cur.z, particleNeighborhood, particleRepulsion);
    
    //Definitely add a directional force option... in either just x / y / z... will be lighter on the system....
    //backBurnerSystem.addAttractionForce(ofRandom(0,cubeResolution), ofRandom(0,cubeResolution), 0, 1000, 0.1);
    
    //Why is the fading thing not working?? figure out
    //Fade particles out over time so does not look abrupt
    if(backBurnerSystem.size() > 0 ){//&& ofGetFrameNum()%5 == 0
        int rand = ofRandom(0,backBurnerSystem.size());
//        int range = MIN(backBurnerSystem.size(), rand+10);
//        for(int i = rand; i < range; i++) {
            if(backBurnerSystem[rand].life == 100000) backBurnerSystem[rand].setLife(255); //CHANGE THIS
//        }
    }

    for(int i = 0; i < backBurnerSystem.size(); i++) {
        BinnedParticle& cur = backBurnerSystem[i];
        if(cur.z > 0) {
            backBurnerSystem.force(cur,cur.x,cur.y,0, 10000, -.01);
        }
        
        if( backBurnerSystem[i].life <= 0) {// clean up particle system (may be a memory leak somewhere)
//            cout << backBurnerSystem[i].life << endl;
            backBurnerSystem.removeAtIndex(i);
        }
        cur.waveFloor(0, 0, backBurnerSystem.getWidth(), backBurnerSystem.getHeight()); // needs work
    }
    
    
    //ACTIVE
    particleSystem.update(ofGetLastFrameTime());
    //BACKGROUND
    backBurnerSystem.update(ofGetLastFrameTime());
    
}


//Keep only display in here so that can swivel around a static state
void Control::draw(){
    particleSystem.draw();
    backBurnerSystem.draw();

    if(currentFrame.mesh.getVertices().size() > 0 && currentFrame.renderMesh){
        currentFrame.mesh.drawWireframe();
    }
    
}


//Storing the latest data in a "current Frame" so that there is an additive effect,
//old data stays on Data Screen
//Also images/meshes are not saved in memory but re-loaded when needed
void Control::loadFrame(){
    
    //Assume all frames will have this basic data
    currentFrame.frameType = sequence.back()->frameType;
    currentFrame.totalTime = sequence.back()->totalTime;
    
    currentFrame.collection = sequence.back()->collection;
    currentFrame.collectionDesc = sequence.back()->collectionDesc;
//    int encounterDate;
//    float encounterLocLong;
//    float encounterLocLat;
    currentFrame.author = sequence.back()->author;
    currentFrame.citation = sequence.back()->citation;
    
    currentFrame.title = sequence.back()->title;
    currentFrame.desc = sequence.back()->desc;
    currentFrame.material = sequence.back()->material;
    currentFrame.historyVec = sequence.back()->historyVec;
    
    switch(currentFrame.frameType){
        case 1 : { // Flocking
            
            if(sequence.back()->leader != -1){
                currentFrame.leader = sequence.back()->leader;
            }else{
                currentFrame.leader = ofRandom(0,sequence.back()->particles.size());
            }
            
            break;
        }
        case 2 : { // Mesh and Colors

            //Boolean to draw full mesh in addition to particle sys version
            currentFrame.renderMesh = sequence.back()->renderMesh;
            
//            if(sequence.back()->mesh.getVertices().size() > 0 ){ //&& sequence.back()->particles.size() == 0
            
                currentFrame.mesh = read.readMesh("meshes/"+sequence.back()->externalFileName);
            
                for(int i = 0; i < currentFrame.mesh.getVertices().size();i++){
                    currentFrame.mesh.setVertex(i, currentFrame.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution*.8,cubeResolution/2));
                }
            
                
                vector<BinnedParticle> tempVec = particleSystem.getParticles();;
                for(int i = 0; i < tempVec.size(); i++ ){
                    backBurnerSystem.add(tempVec[i]);
                }
                
                particleSystem.clear();
                
                
                for (int i = 0; i < currentFrame.mesh.getIndices().size()-3; i+=3){
                    
                    
                    //Rather than having random positioning
                    //get a previous particle position that is being loaded out of the system and if
                    //size() is large add random.
                    
                    float x = ofRandom(0, cubeResolution) ;
                    float y = ofRandom(0, cubeResolution) ;
                    float z = ofRandom(0, cubeResolution) ;
                    BinnedParticle particle(x, y, z, 0, 0, 0);
                    
                    ofVec3f t = currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i));
                    
                    particle.setTarget(t.x,t.y,t.z);
                    
                    ofVec3f diff1 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+1));
                    ofVec3f diff2 = t - currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i+2));
                    
                    particle.p1 = diff1;
                    particle.p2 = diff2;
                    
                    particle.targetColor = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i));
                    particle.p1Color = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+1));
                    particle.p2Color  = currentFrame.mesh.getColor( currentFrame.mesh.getIndex(i+2));
                    
                    particleSystem.add(particle); //unnesseccary storage of the particle arrays / for memory that is
                    
                }
                
//            }
            
            //find best wat to do this but for now ok.
            
            //Hmmm maybe get rid of this.... rather than re-storing the particle data just start from scratch again
            //Only keep a buffer of frames
            //Then reload if you need more...
            
            //check which frame's particles are currently loaded into the system
            //get a copy of the particles and replace the original frame's particles with modified
            //particles. will never render the frame's particles in system.
            
            //instead of keeping the old position of data
            //You are always loading in
            //Always exploding
            //instead just putting into backburner system as flotsam
            
//            for(int i = 0;i < sequence.size();i++){
//                if(sequence[i]->uID == particleSystem.getOwner()) {
//                    sequence[i]->particles = particleSystem.getParticles();
//                    for(int j = 0; j < sequence[i]->particles.size(); j++ ){
//                        backBurnerSystem.add(sequence[i]->particles[j]);
////                        backBurnerSystem[i].setTarget(backBurnerSystem[i].x,backBurnerSystem[i].y,0);
//                    }
//                    particleSystem.clear();
//                    break;
//                }
//            }
            
            
//            vector<BinnedParticle> tempVec = particleSystem.getParticles();;
//            for(int i = 0; i < tempVec.size(); i++ ){
//                backBurnerSystem.add(tempVec[i]);
//            }
//
//            particleSystem.clear();
//
//
//
//            for (int i = 0; i < sequence.back()->particles.size(); i++){
//                particleSystem.add(sequence.back()->particles[i]);
//            }
            
//            particleSystem.setOwner(sequence.back()->uID);//maybe still nice to preserve this info ?
            
            break;
        }
        case 3 : { // Mesh
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
    
    
    dataScreen.loadData(currentFrame);
    
}


//Add in more secondScreen data draw functions here

void Control::setupDataScreen(){
    dataScreen.setup();
    
}

void Control::drawDataScreen(){
    
    dataScreen.draw();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(particleSystem.size()) + " particles", 32, 32);
    ofDrawBitmapString(ofToString(backBurnerSystem.size()) + " backburner particles", 32, 52);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 72);
    ofDrawBitmapString(ofToString((int) (ofGetElapsedTimeMillis() - startTime)) + " millis",32,92);
}

void Control::exportPLY(){
    frame temp;
    
    //for export it's not just the current frame, it's the whole system
    //move into JsonRW / equivalent name eventually
    
    for(int i = 0; i < particleSystem.size(); i++){
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
    
    for(int i = 0; i < backBurnerSystem.size(); i++){
            ofVec3f p = ofVec3f(backBurnerSystem[i].x,backBurnerSystem[i].y,backBurnerSystem[i].z);
            ofVec3f p1 = ofVec3f(p.x + backBurnerSystem[i].p1.x, p.y + backBurnerSystem[i].p1.y, p.z + backBurnerSystem[i].p1.z);
            ofVec3f p2 = ofVec3f(p.x + backBurnerSystem[i].p2.x, p.y + backBurnerSystem[i].p2.y, p.z + backBurnerSystem[i].p2.z);
            if(p.distance(p1) < 30 && p.distance(p2) < 30){
                temp.pointsB.push_back(p);
                temp.pointsB.push_back(p1);
                temp.pointsB.push_back(p2);
                temp.pointColorsB.push_back(backBurnerSystem[i].targetColor);
                temp.pointColorsB.push_back(backBurnerSystem[i].p1Color);
                temp.pointColorsB.push_back(backBurnerSystem[i].p2Color);
            }
    }

    write.writeMesh(temp,"pointCloud_"+ofGetTimestampString());
}

