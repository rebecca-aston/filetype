//
//  Control.cpp
//  artifactlabEMMERSION
//
//  Created by Rebecca Aston on 7/17/18.
//

#include "Control.h"

Control::Control(){
    
}

// set web images in another directory that are always just being looped through and displayed
// I think maybe try and grab an image that "looks good" like could trace image to get a "whole" mesh OR just make it random to throw into system.



void Control::setup(){
    

    flock.frameType = 1;
    flock.totalTime = 10000;
    
    //Set up all data to be used for historical / raw data
    read.readModel();
    
    //Store original order of data as unique identifier to be used for comparison later
    for(int i = 0; i < read.frameVec.size(); i++){
        read.frameVec[i].uID = i;
    }
    
    //Initialize the sequence to one random frame from Model
    sequence.push_back(&read.frameVec[ofRandom(read.frameVec.size())]);
    
    
    //Set up all data to be used for commentary pieces
//    Use the flocking type as the authorial intent frame...
//    That could also be when you take a snapshot of the Pattern texture thing.
//    sequence.push_back(&flock);
    
    //Initialize image roulette which will add images scraped from the internet into
    //particle system as a "texture" / fragments in a 3D space
    imgRoulete.setupRoulete();
    
    //Data Screen and Sound Manager do not need to be initialized.
    
    
    //Initialize Particle Systems
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
        
        for(int i = 0; i < sequence.size();i++){
            cout << "CURRENT: "+currentFrame.title+" INDEX: "+ofToString(i) << endl;
        }
        
        loadFrame();
        sequencer();
    }else{// maybe if getting too much lag with everything happening at load do sequencer half way
        if(ofGetElapsedTimeMillis() - startTime > currentFrame.totalTime){
            currentFrame.animating = false;
        }
    }
    
    //Main particle system that is the sort of ACTIVE state
    //which the new frame is loading into.
    //Then there is another BACKGROUND "system" which is the scattered accumulation of particles
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

                //Animate the camera to look at the "leader" particle in the break up of the mesh
                camLook = ofVec3f(particleSystem[currentFrame.leader].x,particleSystem[currentFrame.leader].y,particleSystem[currentFrame.leader].z);

                for(int i = 0; i < particleSystem.size(); i++) {
                    BinnedParticle& cur = particleSystem[i];
                    
//                    cur.addDampingForce();
                }
        
                //Only apply flocking to a random selection of particles
                for(int i = MAX(0,currentFrame.leader - 50); i < MIN(particleSystem.size(),currentFrame.leader + 50); i++){
                    BinnedParticle& cur = particleSystem[i];
                    vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);

                    for(int i = 0; i < neighbors.size(); i++){
                        cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
                    }

                    particleSystem.addRepulsionForce(cur, 70, .1);
                    particleSystem.addAttractionForce(cur, 50, .3);

                }

             //this is causing bugs
                particleSystem.addAttractionForce(particleSystem[currentFrame.leader], particleNeighborhood+30, .3);
                
                break;
            }
            case 2 : { // Mesh and Colors in active particle sys
                
               camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                // Not really using this at the moment
                //Move to index if not there
//                for(int i = 0; i < particleSystem.size(); i++) {
//                    BinnedParticle& cur = particleSystem[i];
//                    particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
//                }

                break;
            }
            case 3 : { // Mesh draw
                
                break;
            }
            case 4 : {
                 camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                break;
            }
            default :
                break;
                
        }
    
    
    //ACTIVE
    //FOR All states
    for(int i = 0; i < particleSystem.size(); i++) {
        BinnedParticle& cur = particleSystem[i];
 
        cur.bounceOffFloor(0);
    }
    
    //BACKGROUND
    
    //Maybe better to think of this background as the flow field / global forces (although ff might be too heavy)
    //Map data / trade routes getting thrown in as movement/flow vectors..
    
    //        write better flow field
    //        particleSystem.flowField(cur.x, cur.y, cur.z, particleNeighborhood, particleRepulsion);
    
    
    
    //BACKGROUND
    
    //FIX FADE!!!
    if(backBurnerSystem.size() > 0 ){//&& ofGetFrameNum()%5 == 0
        int rand = ofRandom(0,backBurnerSystem.size());
//        int range = MIN(backBurnerSystem.size(), rand+10);
//        for(int i = rand; i < range; i++) {
            if(backBurnerSystem[rand].life == 100000) backBurnerSystem[rand].setLife(255); //CHANGE THIS
//        }
    }

    for(int i = 0; i < backBurnerSystem.size(); i++) {
        BinnedParticle& cur = backBurnerSystem[i];
        
        //Make particles float down to floor
        if(cur.z > 0) {
            backBurnerSystem.force(cur,cur.x,cur.y,0, 10000, -.01);
        }
        
        //FADE / more elegant system?
        if( backBurnerSystem[i].life <= 0) {
            backBurnerSystem.removeAtIndex(i);
        }

        cur.bounceOffFloor(0);
    }
    
    
    //ACTIVE
    particleSystem.update(ofGetLastFrameTime());
    //BACKGROUND
    backBurnerSystem.update(ofGetLastFrameTime());
    
    
    //SoundManager
    
    //Move all this into the sound manager
    //pass in the history vector as the argument
    
    
    soundManager.update(&currentFrame);
    
    
    if(ofGetFrameNum() % 60 == 0){ // && if we are not still adding texture to system
        //get fbo from imgRoulete and draw
        
        imgRoulete.processImage();
        dataScreen.drawImageRoulette(&imgRoulete.imgFbo);
        if(imgRoulete.addTextureToSystem){
            //Get mesh that has been generated 
            //Create new frame of type mesh
            
            //Jump into sequencer to push frame on back/next
            //maybe wait for end of animation to set imgRoulete.addTextureToSystem to false again
            
            cout << "ADDED TO PARTICLE SYSTEM" << endl;
            
            
        }
        
    }
    
    
}


//Keep only display in here so that can swivel around a static state
void Control::draw(){
    particleSystem.draw();
    backBurnerSystem.draw();

    if(currentFrame.mesh.getVertices().size() > 0 && currentFrame.renderMesh){
        currentFrame.mesh.drawWireframe();
    }
    
}

bool compareByWeight(const frame &a, const frame &b){
    return a.weight > b.weight;
}

bool in_array(const std::string &value, const std::vector<string> &array){
    return std::find(array.begin(), array.end(), value) != array.end();
}

//A sequencer of the content that is a weighted random choice based off of tags
//i.e. loose association to the current frame
void Control::sequencer(){
    
    for(int i = 0; i < read.frameVec.size(); i++){
        float w = read.frameVec[i].weight;
        w *= 0.1;
        
        for(int j = 0; j < read.frameVec[i].tags.size(); j++){
            if(read.frameVec[i].uID != currentFrame.uID){
                if(in_array(read.frameVec[i].tags[j],currentFrame.tags)){
                    
                    w += 1/float(currentFrame.tags.size());
                    
                }
            }
        }
    
        read.frameVec[i].weight = MIN(w,1);

    }
    
    ofSort(read.frameVec, &compareByWeight);
    
     for(int i = 0; i < read.frameVec.size(); i++){
         cout << read.frameVec[i].title ;
         cout << ": "+ofToString(read.frameVec[i].weight) << endl;
     }
    
    //Randomly choose from the first quarter of strongest weighted frames
    int weightedRandom = ofRandom(ceil(read.frameVec.size()/2));
    if(read.frameVec[weightedRandom].uID == currentFrame.uID) weightedRandom ++; //leave in just in case get repetition
    sequence.push_front(&read.frameVec[weightedRandom]);
    
    sequence.pop_back();
    
}



void Control::shiftFrame(){
    
    //Add in a function that removes end and adds new from data
    sequence.pop_back();
    
    loadFrame();
}


//Storing the latest data in a "current Frame" so that there is an additive effect,
//old data stays on Data Screen
//Also images/meshes are not saved in memory but re-loaded when needed
void Control::loadFrame(){
    
    //Assume all frames will have this basic data
    currentFrame.uID = sequence.back()->uID;
    currentFrame.frameType = sequence.back()->frameType;
    currentFrame.weight = sequence.back()->weight;
    currentFrame.tags = sequence.back()->tags;
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
    currentFrame.currentHistoryIndex = -1;
    
    
    switch(currentFrame.frameType){
        case 1 : { // Flocking
            
            if(particleSystem.size() == 0){
                
                shiftFrame();
                
            }else { //if(currentFrame.leader == -1)
                currentFrame.leader = ofRandom(0,particleSystem.size());
            }
            
            break;
        }
        case 2 : { // Mesh and Colors

            //Boolean to draw full mesh in addition to particle sys version
            currentFrame.renderMesh = sequence.back()->renderMesh;
            
//            ofVec3f totalPos;
//
//            for(int i = 0; i < currentFrame.mesh.getVertices().size(); i ++){
//                totalPos += currentFrame.mesh.getVertex(i);
//            }
            
//            camLook = totalPos/currentFrame.mesh.getVertices().size();
            
            //I think randomParticle is better, just need to debug.
            
            currentFrame.mesh = read.readMesh("meshes/"+sequence.back()->externalFileName);
            
            addMeshToParticleSys();
            
            //For camera to look at
            randParticle = ofRandom(particleSystem.size());
            
            break;
        }
        case 3 : { // Mesh
//            currentFrame.mesh = sequence.back()->mesh;
//            currentFrame.renderMesh = sequence.back()->renderMesh;
            
            break;
        }
        case 4 : { // Texture
            
            //Create and dispose of delaunay iamge here
            DelaunayImage del;
            
            currentFrame.image = read.readImage("textures/"+sequence.back()->externalFileName);
            
            camLook = ofVec3f(cubeResolution/2,cubeResolution/2,cubeResolution/2);
            
            currentFrame.mesh = del.triangulateImage(currentFrame.image);
            
            addMeshToParticleSys();
            
            //For camera to look at
            randParticle = ofRandom(particleSystem.size());
            
            break;
        }
        case 5 : { // Text
            //If it's only text based, let the sound keep playing in the background?
            currentFrame.totalTime = 500;
            
            if(sequence.back()->externalFileName != "" && (sequence.back()->externalFileName.find(".png") != std::string::npos || sequence.back()->externalFileName.find(".jpg") != std::string::npos || sequence.back()->externalFileName.find(".jpeg") != std::string::npos)){
                currentFrame.image = read.readImage("images/"+sequence.back()->externalFileName);
            }
            
            break;
        }
        default : {
            //error handling if an incomplete json file ends up in data
            cout << "Unrecognized Frame Type" << endl;
            //Dispose of frame and continue with deque in next call to update
            shiftFrame();
            
            break;
        }
            
    }
    
    
    if(currentFrame.historyVec.size() > 0){
        for(int i = 0; i < currentFrame.historyVec.size(); i ++){
            int timeAprox = int(currentFrame.historyVec[i].text.length()*70);
            
            currentFrame.historyVec[i].length = timeAprox;
            
            //Do this in case the text is very long??
            currentFrame.totalTime += timeAprox;
        }
    }
    
    
    dataScreen.loadData(currentFrame);
    
}


void Control::addMeshToParticleSys(){
    
    for(int i = 0; i < currentFrame.mesh.getVertices().size();i++){
        currentFrame.mesh.setVertex(i, currentFrame.mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution/2,cubeResolution/2));
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
        
//        float x = ofRandom(0, cubeResolution) ;
//        float y = ofRandom(0, cubeResolution) ;
//        float z = ofRandom(0, cubeResolution) ;
//        BinnedParticle particle(x, y, z, 0, 0, 0);
        
        ofVec3f t = currentFrame.mesh.getVertex( currentFrame.mesh.getIndex(i));
        
        BinnedParticle particle(t.x, t.y, t.z, 0, 0, 0);
        
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
        if(particleSystem[i].x < (cubeResolution/2)-120){
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
    
//    for(int i = 0; i < particleSystem.size(); i++){
//        ofVec3f p = ofVec3f(particleSystem[i].x,particleSystem[i].y,particleSystem[i].z);
//        ofVec3f p1 = ofVec3f(p.x + particleSystem[i].p1.x, p.y + particleSystem[i].p1.y, p.z + particleSystem[i].p1.z);
//        ofVec3f p2 = ofVec3f(p.x + particleSystem[i].p2.x, p.y + particleSystem[i].p2.y, p.z + particleSystem[i].p2.z);
//        if(p.distance(p1) < 30 && p.distance(p2) < 30){
//            temp.points.push_back(p);
//            temp.points.push_back(p1);
//            temp.points.push_back(p2);
//            temp.pointColors.push_back(particleSystem[i].targetColor);
//            temp.pointColors.push_back(particleSystem[i].p1Color);
//            temp.pointColors.push_back(particleSystem[i].p2Color);
//        }
//    }
    
    
    
//    for(int i = 0; i < backBurnerSystem.size(); i++){
//            ofVec3f p = ofVec3f(backBurnerSystem[i].x,backBurnerSystem[i].y,backBurnerSystem[i].z);
//            ofVec3f p1 = ofVec3f(p.x + backBurnerSystem[i].p1.x, p.y + backBurnerSystem[i].p1.y, p.z + backBurnerSystem[i].p1.z);
//            ofVec3f p2 = ofVec3f(p.x + backBurnerSystem[i].p2.x, p.y + backBurnerSystem[i].p2.y, p.z + backBurnerSystem[i].p2.z);
//            if(p.distance(p1) < 30 && p.distance(p2) < 30){
//                temp.pointsB.push_back(p);
//                temp.pointsB.push_back(p1);
//                temp.pointsB.push_back(p2);
//                temp.pointColorsB.push_back(backBurnerSystem[i].targetColor);
//                temp.pointColorsB.push_back(backBurnerSystem[i].p1Color);
//                temp.pointColorsB.push_back(backBurnerSystem[i].p2Color);
//            }
//    }

    write.writeMesh(temp,"pointCloud_"+ofGetTimestampString());
}

