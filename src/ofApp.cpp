#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(100);
    
//    meshSystem.setup(1000,100,"DecimatedJug.ply");//DinningRoom.ply
    
    //GUI
    gui.setup();
    
    //Button listeners calling functions below
    exportMeshButton.addListener(this, &ofApp::exportMeshButtonPressed);
    addForceButton.addListener(this, &ofApp::addForceButtonPresssed);

    //Toggles declared in ofApp.h
    gui.add(toggle_axis.setup("axis", true));
    gui.add(toggle_grid.setup("grid", true));
    gui.add(wire_frame.setup("wire frame", true));
    
    //Buttons declared in ofApp.h
    gui.add(exportMeshButton.setup("Export Mesh"));
    gui.add(addForceButton.setup("Add Force"));
    
//    //General controls
//    gui.add(control.is_recording);
//    gui.add(control.testing_mode);
//    gui.add(control.paused);
    
    //Inputs OR maybe have the mouse be able to test this...
//    gui.add(control.CONTROLX);
//    gui.add(control.CONTROLY);
//    gui.add(control.CONTROLZ);
    
    //Outputs from Wekinator
    gui.add(meshSystem.SIZE);
    gui.add(meshSystem.PROBABILITY);
    gui.add(meshSystem.SPEED);
    gui.add(meshSystem.GRAVITY);
    gui.add(meshSystem.SIDEWAYS);
    gui.add(meshSystem.CHECKDIST);
    
    
    glShadeModel(GL_FLAT);
    light.setAreaLight(20, 20);
    light.setPosition(0, -1000, 0);
    
    
    
    
    // this number describes how many bins are used
    // on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
    // if this number is too high, binning is not effective
    // because the screen is not subdivided enough. if
    // it's too low, the bins take up so much memory as to
    // become inefficient.
    int binPower = 6;
    cubeResolution = 2000;
    particleSystem.setup(cubeResolution, cubeResolution, binPower);
    
    kBinnedParticles = 3000;
    for(int i = 0; i < kBinnedParticles; i++) {
        float x = ofRandom(0, cubeResolution) ;
        float y = ofRandom(0, cubeResolution) ;
        float z = ofRandom(0, cubeResolution) ;
        BinnedParticle particle(x, y, z, 0, 0, 0);
        particleSystem.add(particle);
    }
    
    timeStep = 100;
    isMousePressed = false;
    slowMotion = true;
    particleNeighborhood = 64;
    particleRepulsion = .5;
    centerAttraction = .01;
    drawBalls = true;

}

//--------------------------------------------------------------
void ofApp::update(){
//    meshSystem.update(ofVec3f(0,0,0)); //This was the controller as attractor
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    particleSystem.setTimeStep(timeStep);
    
    ofSetColor(255);
    
    // do this once per frame
    particleSystem.setupForces();

    // apply per-particle forces
    if(!drawBalls) {
        ofSetLineWidth(8);
        glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
    }
    for(int i = 0; i < particleSystem.size(); i++) {
        BinnedParticle& cur = particleSystem[i];
        // global force on other particles
        particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        // forces on this particle
        cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
        cur.addDampingForce();
    }
    if(!drawBalls) {
        glEnd();
    }
    
    // single-pass global forces
    if(!isMousePressed){
        particleSystem.addAttractionForce(cubeResolution / 2, cubeResolution / 2, cubeResolution / 2, cubeResolution * 100, centerAttraction);
    }
        
    if(isMousePressed){
//     particleSystem.addAttractionForce(cubeResolution * ofRandom(0,1), cubeResolution * ofRandom(0,1), cubeResolution * ofRandom(0,1), 200, 1);
    particleSystem.addAttractionForce(cubeResolution * 0.2, cubeResolution * 0.2, cubeResolution * 0.2, 50000, 1);
    }

    particleSystem.update(ofGetLastFrameTime());
    

    
//    ofPopMatrix();
    

    
    
    
    
    
    //enables depth test for 3D camera
    ofEnableDepthTest();
   
    //Do any 3D drawing inside of easy cam instance
    cam.begin();
    
    if(toggle_axis){
        drawAxis();
    }
    if(toggle_grid){
        drawGrid();
    }
    
    ofEnableLighting();
    light.enable();
   
//    meshSystem.draw(wire_frame);
    
    // draw all the particles
    if(drawBalls) {
        for(int i = 0; i < particleSystem.size(); i++) {
//            ofCircle(particleSystem[i].x, particleSystem[i].y, particleNeighborhood * .3);
            ofDrawSphere(particleSystem[i].x, particleSystem[i].y, particleSystem[i].z, particleNeighborhood * .3);
        }
    }
    
    
    light.disable();
    ofDisableLighting();
    
    cam.end();
    
    //disables depth so gui text is visible in 2D in top of 3D cam
    ofDisableDepthTest();
    ofSetColor(255);
    gui.draw();

    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
    

}

//--------------------------------------------------------------
void ofApp::drawAxis() {
    ofPushStyle();
    // X
    ofSetLineWidth(3);
    ofSetColor(255, 0, 0);
    ofDrawLine(0, 0, 0, 100, 0, 0);

    // Y
    ofSetLineWidth(3);
    ofSetColor(0, 255, 0);
    ofDrawLine(0, 0, 0, 0, 100, 0);

    // Z
    ofSetLineWidth(3);
    ofSetColor(0, 0, 255);
    ofDrawLine(0, 0, 0, 0, 0, 100);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGrid() {
    ofPushStyle();
    ofPushMatrix();
    ofSetLineWidth(1);
    ofSetColor(200);
    float sz = 100;
    int nm = 8;
    // draw lines along the flat XY meshSystem
    for(int i = 0; i <= nm; i++){
        ofDrawLine( (i * sz) - (sz*nm/2), -sz*nm/2, 0, (i * sz) - (sz*nm/2), sz*nm/2, 0);
        ofDrawLine( -sz*nm/2, (i * sz) - (sz*nm/2), 0, sz*nm/2, (i * sz) - (sz*nm/2), 0);
    }
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exportMeshButtonPressed(){
    meshSystem.exportMesh("artifact_"+ofGetTimestampString()+".ply");
    
}

//--------------------------------------------------------------
void ofApp::addForceButtonPresssed(){

    isMousePressed = !isMousePressed;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ //This is for the DTW training

}
