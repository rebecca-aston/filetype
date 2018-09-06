#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    control.setup();
    
    glShadeModel(GL_FLAT);
    light.setAreaLight(20, 20);
    light.setPosition(0, -1000, 0);
        
}
void ofApp::setupSecondScreen(){
    
    //If need to add gui params they should be available
    control.setupDataScreen();
    
    //GUI
    gui.setup();
    
    //Button listeners calling functions below
    exportMeshButton.addListener(this, &ofApp::exportMeshButtonPressed);
    
    
    //Toggles declared in ofApp.h
    gui.add(toggle_axis.setup("axis", true));
    gui.add(toggle_grid.setup("grid", true));
    gui.add(pause.setup("pause", false));
    gui.add(presentationMode.setup("presentation mode", false));
    
    
    //Buttons declared in ofApp.h
    gui.add(exportMeshButton.setup("Export Mesh"));

    
}


//--------------------------------------------------------------
void ofApp::update(){
    if(!pause){
        control.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //enables depth test for 3D camera
    ofEnableDepthTest();
    
    if(presentationMode){
        
        //Get it to always look at the center or maybe at leader or random particle
        cam.lookAt(control.camLook,ofVec3f(0,0,1));//second argument is up vector
        
        //Just for a little movement through / around / into the meshes
        //Wanted to get that real sense of changing scale where some times they are just triangles of color filling
        //the whole screen, and at others you see a whole jug in the distance
        float orbitRadius = ofMap(sin(ofGetFrameNum()*0.002),-1,1,400,900);
        float ypos= sin(ofDegToRad(displayCamRotation))*orbitRadius;
        float xpos= cos(ofDegToRad(displayCamRotation))*orbitRadius;

        cam.setGlobalPosition(xpos,ypos, 700);

        if(displayCamRotation > 360){
            displayCamRotation = 0;
        }else{
            displayCamRotation += 0.1;
        }
        
    }
    
    
    //Do any 3D drawing inside of easy cam instance
    cam.begin();
    
    if(toggle_axis && !presentationMode){
        drawAxis();
    }
    if(toggle_grid && !presentationMode){
        drawGrid();
    }

    control.draw();
   
    cam.end();
    
}

void ofApp::drawSecondScreen(ofEventArgs & args){

    control.drawDataScreen();
    
    ofSetColor(255);
    gui.draw();
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
 
    for(int i = 0; i <= nm; i++){
        ofDrawLine( (i * sz) - (sz*nm/2), -sz*nm/2, 0, (i * sz) - (sz*nm/2), sz*nm/2, 0);
        ofDrawLine( -sz*nm/2, (i * sz) - (sz*nm/2), 0, sz*nm/2, (i * sz) - (sz*nm/2), 0);
    }
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exportMeshButtonPressed(){
    cout << "Exporting" << endl;
    control.exportPLY();
}

//--------------------------------------------------------------
void ofApp::addForceButtonPresssed(){
//    isMousePressed = !isMousePressed;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
