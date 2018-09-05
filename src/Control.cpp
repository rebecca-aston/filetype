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
    
    camLook = ofVec3f(500,500,500);
    
    //Set up all data to be used for commentary pieces
    //    Use the flocking type as the authorial intent frame...
    //    That could also be when you take a snapshot of the Pattern texture thing.
    //    sequence.push_back(&flock);
    scatter.frameType = 1;
    scatter.totalTime = 20000;
    
    //Set up all data to be used for historical / raw data
    read.readModel();
    
    if(read.imageFrameVec.size() > 0){
        Sequencer imageSequence;
        imageSequence.setup(read.imageFrameVec, "image");
        
        sequences.push_back(imageSequence);
    }

    if(read.textFrameVec.size() > 0){
        Sequencer textSequence;
        textSequence.setup(read.textFrameVec, "text");
        
        sequences.push_back(textSequence);
    }
    
    if(read.processFrameVec.size() > 0){
        Sequencer processSequence;
        processSequence.setup(read.processFrameVec, "process");
        
        sequences.push_back(processSequence);
    }
    
    if(read.threeDFrameVec.size() > 0){
        Sequencer threeDSequence;
        threeDSequence.setup(read.threeDFrameVec, "3D");
        
        sequences.push_back(threeDSequence);
    }
    
    //Free up memory
    read.clearVectors();
    

    
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

//Setup Data Screen
void Control::setupDataScreen(){
    dataScreen.setup();
}

void Control::update(){


    for(int i = 0; i < sequences.size();i++){
        if(!sequences[i].currentFrame.animating){
            sequences[i].startTime = ofGetElapsedTimeMillis(); 
            sequences[i].currentFrame.animating = true;
            
            cout << "CURRENT: "+sequences[i].currentFrame.title+sequences[i].vecType<< endl;
            
            //Every other frame in the mesh field, add a scatter frame
            if(sequences[i].vecType == "3D" && !scattering){
//                scattering = true;
                sequences[i].forceNewFrame(scatter, 1); //Scaterring type
            }
//            else if(sequences[i].vecType == "3D" ){
//                scattering = false;
//            }
            
            sequences[i].sequencer();
            loadFrame(&sequences[i]);
            
        }else{// maybe if getting too much lag with everything happening at load do sequencer half way
            if(ofGetElapsedTimeMillis() - sequences[i].startTime > sequences[i].currentFrame.totalTime){
                sequences[i].currentFrame.animating = false;
            }
        }
        
    }

    
    //Just for audio tracks not system read out
    soundManager.updateVolumes();
    
    //Particle System
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
    
    
    //Change to two particle system states
    
    if(scattering){

        //Animate the camera to look at the "leader" particle in the break up of the mesh
//        camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
        
//        for(int i = 0; i < particleSystem.size(); i++) {
//            BinnedParticle& cur = particleSystem[i];
//
//            //                    cur.addDampingForce();
//        }
        
        //Only apply flocking to a random selection of particles
        for(int i = MAX(0,randParticle - 100); i < MIN(particleSystem.size(),randParticle + 100); i++){
            BinnedParticle& cur = particleSystem[i];
            vector<BinnedParticle*> neighbors = particleSystem.getNeighbors(cur,60);
            
            for(int i = 0; i < neighbors.size(); i++){
                cur.align(neighbors[i]->xv, neighbors[i]->yv, neighbors[i]->zv);
            }
            
            particleSystem.addRepulsionForce(cur, 70, .1);
            particleSystem.addAttractionForce(cur, 50, .3);
            
        }
        
        //this is causing bugs
        particleSystem.addAttractionForce(particleSystem[randParticle], particleNeighborhood+30, .3);
        
    }else{
        
//        camLook = ofVec3f(particleSystem[randParticle].xt,particleSystem[randParticle].yt,particleSystem[randParticle].zt);
        
        // Not really using this at the moment
        //Move to index if not there
        for(int i = 0; i < particleSystem.size(); i++) {
            BinnedParticle& cur = particleSystem[i];
            particleSystem.force(cur,cur.xt,cur.yt,cur.zt, 10000, -.01);
            cur.addDampingForce();
        }
        
        
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
    

    
    
}


//Draw Particle System Screen
void Control::draw(){
    backBurnerSystem.draw();
    particleSystem.draw();
}

//Draw Data Screen
void Control::drawDataScreen(){
    
    dataScreen.draw(&particleSystem, &backBurnerSystem);
    
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(particleSystem.size()) + " particles", ofGetWidth()/2+32, 32);
    ofDrawBitmapString(ofToString(backBurnerSystem.size()) + " backburner particles", ofGetWidth()/2+32, 52);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", ofGetWidth()/2+32, 72);
    ofPopStyle();
}

//Storing the latest data in a "current Frame" so that there is an additive effect,
//old data stays on Data Screen
//Also images/meshes are not saved in memory but re-loaded when needed
void Control::loadFrame(Sequencer * s){
    
    s->resetCurrentFrame();
    
    switch(s->currentFrame.frameType){
        case 1 : { // Process
          
            
            if(particleSystem.size() == 0){//Old error???
                
                s->sequencer();
                loadFrame(s);
                
            }else { //if(scatterSource == -1)
                
                scattering = true;
            }
            
            break;
        }
        case 2 : { // Mesh and Colors
            
            
            if(s->currentFrame.uID.find("3D") != std::string::npos){
                s->currentFrame.mesh = read.readMesh("meshes/"+s->currentFrame.externalFileName);
            }
      
            if(s->currentFrame.mesh.getVertices().size() > 0){
                addMeshToParticleSys(s->currentFrame.mesh);
                
                //For camera to look at
                randParticle = ofRandom(particleSystem.size());
                camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                scattering = false;
            }

            break;
        }
        case 3 : { // Image
//            currentFrame.mesh = sequence.back()->mesh;
//            currentFrame.renderMesh = sequence.back()->renderMesh;
            s->currentFrame.image = read.readImage("images/"+s->currentFrame.externalFileName);
            
            if(s->currentFrame.image.isAllocated()){
                if(ofRandom(1)>0.96){
                    DelaunayImage del01;
                    s->currentFrame.mesh = del01.triangulateImage(s->currentFrame.image,false,cubeResolution);
                    
                    s->currentFrame.totalTime = 4000; //SLow the image from moving off?
                    
    //                Sequencer * s3D = getSequenceByType("3D");
    //                s3D->forceNewFrame(s->currentFrame,2); //returns a pointer
                    addMeshToBackBurnerSys(s->currentFrame.mesh);
                }
            }else{
                cout << "Image load failed: ";
                cout << s->currentFrame.externalFileName << endl;
                s->sequencer();
                loadFrame(s);
            }
            
            break;
        }
        case 4 : { // Texture
            
            //Create and dispose of delaunay iamge here
            s->currentFrame.image = read.readImage("textures/"+s->currentFrame.externalFileName);
            
            if(s->currentFrame.image.isAllocated()){
                DelaunayImage del;
                s->currentFrame.mesh = del.triangulateImage(s->currentFrame.image,true,cubeResolution);
                
                
                addMeshToParticleSys(s->currentFrame.mesh);
                
                //For camera to look at
                randParticle = ofRandom(particleSystem.size());
                
                camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                scattering = false;
            }else{
                cout << "Image load failed: ";
                cout << s->currentFrame.externalFileName << endl;
                s->sequencer();
                loadFrame(s);
            }

            break;
        }
        case 5 : { // Text
            //If it's only text based, let the sound keep playing in the background?
            s->currentFrame.totalTime = 500;
            
            //hmm am I still using this check?
            if(s->currentFrame.externalFileName != "" && (s->currentFrame.externalFileName.find(".png") != std::string::npos || s->currentFrame.externalFileName.find(".jpg") != std::string::npos || s->currentFrame.externalFileName.find(".jpeg") != std::string::npos)){
                s->currentFrame.image = read.readImage("images/"+s->currentFrame.externalFileName);
            }
            
            if(s->currentFrame.historyVec.size() > 0){
                for(int i = 0; i < s->currentFrame.historyVec.size(); i ++){
                    int timeAprox = int(s->currentFrame.historyVec[i].text.length()*70);
                    
                    s->currentFrame.historyVec[i].length = timeAprox;
                    
                    //Do this in case the text is very long??
                    s->currentFrame.totalTime += timeAprox;
                }
            }
            
            soundManager.updateReadOut(&s->currentFrame); //Will this allow overlapping? probably not... could strip down
            break;
        }
        default : {
            //error handling if an incomplete json file ends up in data
            cout << "Unrecognized Frame Type" << endl;
            //Dispose of frame and continue with deque in next call to update
            s->sequencer();
            loadFrame(s);
            break;
        }
            
    }
    

    
    soundManager.updateSound(&s->currentFrame);
    
    //Also maybe better to separate soundManager into text read and sound play
    dataScreen.loadData(s->currentFrame);
}


void Control::addMeshToParticleSys(ofMesh mesh){
    
    for(int i = 0; i < mesh.getVertices().size();i++){
        mesh.setVertex(i, mesh.getVertex(i) + ofVec3f(cubeResolution/2,cubeResolution/2,cubeResolution/2));
    }
    
    
    vector<BinnedParticle> tempVec = particleSystem.getParticles();;
    for(int i = 0; i < tempVec.size(); i++ ){
        backBurnerSystem.add(tempVec[i]);
    }
    
    particleSystem.clear();
    
    
    for (int i = 0; i < mesh.getIndices().size()-3; i+=3){
        
        ofVec3f t = mesh.getVertex( mesh.getIndex(i));
        
        BinnedParticle particle(t.x+ofRandom(-50,50), t.y+ofRandom(-50,50), t.z+ofRandom(-50,50), 0, 0, 0);
        
        particle.setTarget(t.x,t.y,t.z);
        
        ofVec3f diff1 = t - mesh.getVertex( mesh.getIndex(i+1));
        ofVec3f diff2 = t - mesh.getVertex( mesh.getIndex(i+2));
        
        particle.p1 = diff1;
        particle.p2 = diff2;
        
        particle.targetColor = mesh.getColor( mesh.getIndex(i));
        particle.p1Color = mesh.getColor( mesh.getIndex(i+1));
        particle.p2Color  = mesh.getColor( mesh.getIndex(i+2));
        
        particleSystem.add(particle); //unnesseccary storage of the particle arrays / for memory that is
        
    }
    
}

//To initialize straight to backburner
void Control::addMeshToBackBurnerSys(ofMesh mesh){
    
    for (int i = 0; i < mesh.getIndices().size()-3; i+=3){
        
        ofVec3f t = mesh.getVertex( mesh.getIndex(i));
        
        BinnedParticle particle(t.x+ofRandom(-200,200),t.y+ofRandom(-200,200),t.z+ofRandom(-200,200), 0, 0, 0);
        
        ofVec3f diff1 = t - mesh.getVertex( mesh.getIndex(i+1));
        ofVec3f diff2 = t - mesh.getVertex( mesh.getIndex(i+2));
        
        particle.p1 = diff1;
        particle.p2 = diff2;
        
        particle.targetColor = mesh.getColor( mesh.getIndex(i));
        particle.p1Color = mesh.getColor( mesh.getIndex(i+1));
        particle.p2Color  = mesh.getColor( mesh.getIndex(i+2));
        
        backBurnerSystem.add(particle); //unnesseccary storage of the particle arrays / for memory that is
        
    }
}

Sequencer* Control::getSequenceByType(string t){
    for(int i = 0; i < sequences.size();i++){
        if(sequences[i].vecType == t) return &sequences[i];
    }
}


















//MOVE THIS TO WRITE
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

