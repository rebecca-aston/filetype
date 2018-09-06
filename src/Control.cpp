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
    
    //This is a frame that represents the act of scattering/exploding
    //It is the sort of artistic act that I am taking and have thought of this as the "process" frame type
    scatter.frameType = 1;
    scatter.totalTime = 20000;
    
    //Set up all the filesystem data to be used for historical / raw data
    read.readModel();
    
//    Grab the data and load it into sequencer objects that manage different categories of data
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
    
//    if(read.processFrameVec.size() > 0){
//        Sequencer processSequence;
//        processSequence.setup(read.processFrameVec, "process");
//
//        sequences.push_back(processSequence);
//    }
    
    //I ended up just adding process into the 3D vector as it's in the same space / particle system
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
    
    //The ACTIVE particles
    particleSystem.setup(cubeResolution, cubeResolution, binPower);
    
    //The BACKGROUND particle system
    backBurnerSystem.setup(cubeResolution*1.5, cubeResolution*1.5, binPower);
    
    timeStep = 100;
 
    particleNeighborhood = 64;
    particleRepulsion = .5;
    particleCohesion = .3;
    centerAttraction = .01;
    particleAlign = 0;
}

//Setup Data Screen
void Control::setupDataScreen(){
    dataScreen.setup();
}

void Control::update(){

    //Loading in a new "frame" of data is handled by animation time variable
    //Each sequence stores a current frame and its data and animation time
    for(int i = 0; i < sequences.size();i++){
        if(!sequences[i].currentFrame.animating){
            sequences[i].startTime = ofGetElapsedTimeMillis(); 
            sequences[i].currentFrame.animating = true;
            
            //Debug
//            cout << "CURRENT: "+sequences[i].currentFrame.title+sequences[i].vecType<< endl;
            
            //Every other frame in the mesh field, add a scatter frame
            if(sequences[i].vecType == "3D" && !scattering){
                sequences[i].forceNewFrame(scatter, 1); //Second argument is Scaterring type
            }

            sequences[i].sequencer();
            loadFrame(&sequences[i]);
            
        }else{
            if(ofGetElapsedTimeMillis() - sequences[i].startTime > sequences[i].currentFrame.totalTime){
                sequences[i].currentFrame.animating = false;
            }
        }
        
    }

    
    //Just for audio tracks not system/text read out
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
        
        particleSystem.addAttractionForce(particleSystem[randParticle], particleNeighborhood+30, .3);
        
    }else{
        
        //Move the the position of the target/original mesh vertex (they are initialized with a bit of randomness so they shiver into place)
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
    //FIX FADE: the life of the particle thing is something I'd like to make more elegant
    if(backBurnerSystem.size() > 0 ){
        int rand = ofRandom(0,backBurnerSystem.size());
            if(backBurnerSystem[rand].life == 100000) backBurnerSystem[rand].setLife(255); //CHANGE THIS
    }

    for(int i = 0; i < backBurnerSystem.size(); i++) {
        BinnedParticle& cur = backBurnerSystem[i];
        
        //Make particles float down to floor
        if(cur.z > 0) {
            backBurnerSystem.force(cur,cur.x,cur.y,0, 10000, -.01);
        }
        
        //Remove particel at end of life
        if( backBurnerSystem[i].life <= 0) {
            backBurnerSystem.removeAtIndex(i);
        }

        cur.bounceOffFloor(0);
    }
    
    
    //ACTIVE
    particleSystem.update(ofGetLastFrameTime());
    //BACKGROUND
    backBurnerSystem.update(ofGetLastFrameTime());
    
}


//Draw Particle System Screen
void Control::draw(){
    backBurnerSystem.draw();
    particleSystem.draw();
}

//Draw Data Screen
void Control::drawDataScreen(){
    
    dataScreen.draw(&particleSystem, &backBurnerSystem);
    
    //Debug
//    ofPushStyle();
//    ofSetColor(255);
//    ofDrawBitmapString(ofToString(particleSystem.size()) + " particles", ofGetWidth()/2+32, 32);
//    ofDrawBitmapString(ofToString(backBurnerSystem.size()) + " backburner particles", ofGetWidth()/2+32, 52);
//    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", ofGetWidth()/2+32, 72);
//    ofPopStyle();
}

void Control::loadFrame(Sequencer * s){
    
    s->resetCurrentFrame();
    
    switch(s->currentFrame.frameType){
        case 1 : { // Process / Explosion / Scatter / Flocking
          
            if(particleSystem.size() == 0){
                
                s->sequencer();
                loadFrame(s);
                
            }else {
                //Manage switching  between scattering  and normal mesh state of particle system
                scattering = true;
            }
            
            break;
        }
        case 2 : { // Mesh / Scan
            
            s->currentFrame.image = read.readImage("images/"+s->currentFrame.material);//realized need an image of mesh to understand correlation between data panel and particle simulation. Bit last minute use of unused data field
    
            if(s->currentFrame.uID.find("3D") != std::string::npos){//only load mesh if it was not originally an image
                s->currentFrame.mesh = read.readMesh("meshes/"+s->currentFrame.externalFileName);
            }
        
            if(s->currentFrame.mesh.getVertices().size() > 0){
                addMeshToParticleSys(s->currentFrame.mesh);
                
                //For animating camera to look at
                randParticle = ofRandom(particleSystem.size());
                camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                //Manage switching  between scattering  and normal mesh state of particle system
                scattering = false;
            }

            break;
        }
        case 3 : { // Image

            s->currentFrame.image = read.readImage("images/"+s->currentFrame.externalFileName);
            s->currentFrame.addToMeta = false;
            
            //Randomly add an image into the Backburner system as fragments of color
            if(s->currentFrame.image.isAllocated()){
                if(ofRandom(1)>0.95){
                    DelaunayImage del01;
                    s->currentFrame.mesh = del01.triangulateImage(s->currentFrame.image,false,cubeResolution);
                    s->currentFrame.addToMeta = true;
                    s->currentFrame.totalTime = 5000;
                    
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
        case 4 : { // " Texture " / fake 3D
            
            //Use Delaunay Triangulation to convert image to fragments of color (wanted to use the same visual langauge as the meshes)
            s->currentFrame.image = read.readImage("textures/"+s->currentFrame.externalFileName);
            
            if(s->currentFrame.image.isAllocated()){
                DelaunayImage del;
                s->currentFrame.mesh = del.triangulateImage(s->currentFrame.image,true,cubeResolution);
                
                
                addMeshToParticleSys(s->currentFrame.mesh);
                
                //For camera to look at
                randParticle = ofRandom(particleSystem.size());
                camLook = ofVec3f(particleSystem[randParticle].x,particleSystem[randParticle].y,particleSystem[randParticle].z);
                
                //Manage switching  between scattering  and normal mesh state of particle system
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
            
            s->currentFrame.totalTime = 500;
            
            if(s->currentFrame.externalFileName != "" && (s->currentFrame.externalFileName.find(".png") != std::string::npos || s->currentFrame.externalFileName.find(".jpg") != std::string::npos || s->currentFrame.externalFileName.find(".jpeg") != std::string::npos)){
                s->currentFrame.image = read.readImage("images/"+s->currentFrame.externalFileName);
            }
            
            //Original plan was to incorporate dates more, but the processing time of all this data just proved too much for now
            if(s->currentFrame.historyVec.size() > 0){
                for(int i = 0; i < s->currentFrame.historyVec.size(); i ++){
                    int timeAprox = int(s->currentFrame.historyVec[i].text.length()*70);
                    
                    s->currentFrame.historyVec[i].length = timeAprox;
                    
                    //Rough approximation of how long text will take to be read by system
                    s->currentFrame.totalTime += timeAprox;
                }
            }
            
            soundManager.updateReadOut(&s->currentFrame);
            break;
        }
        default : {
            //error handling if an incomplete json file ends up in data folder
            cout << "Unrecognized Frame Type" << endl;
            //Dispose of frame and continue with deque in next call to update
            s->sequencer();
            loadFrame(s);
            break;
        }
            
    }
    

    //Load in any audio files that have been added
    soundManager.updateSound(&s->currentFrame);
    
    //update the data screen
    dataScreen.loadData(s->currentFrame);
}

//Add mesh data to the ACTIVE state and move current active particles to the backburner system
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
        
        particleSystem.add(particle);
        
    }
    
}

//To initialize straight to backburner (for add the random images in)
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
        
        backBurnerSystem.add(particle);
        
    }
}

//Didn't really use in the end, but could be helpful in future
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

