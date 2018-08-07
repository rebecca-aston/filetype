#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(200);
    
    control.setup();
    
    
    glShadeModel(GL_FLAT);
    light.setAreaLight(20, 20);
    light.setPosition(0, -1000, 0);
    
}
void ofApp::setupSecondScreen(){
    //GUI
    gui.setup();
    
    //Button listeners calling functions below
    exportMeshButton.addListener(this, &ofApp::exportMeshButtonPressed);
    
    
    //Toggles declared in ofApp.h
    gui.add(toggle_axis.setup("axis", true));
    gui.add(toggle_grid.setup("grid", true));
    gui.add(pause.setup("pause", false));
    
    
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
    
    //Do any 3D drawing inside of easy cam instance
    cam.begin();
    
    if(toggle_axis){
        drawAxis();
    }
    if(toggle_grid){
        drawGrid();
    }
    
//    ofEnableLighting();
//    light.enable();
    

    control.draw();
   
//    light.disable();
//    ofDisableLighting();
    
    cam.end();
    
    //disables depth so gui text is visible in 2D in top of 3D cam
//    ofDisableDepthTest();

}

void ofApp::drawSecondScreen(ofEventArgs & args){
    ofSetColor(255);
    gui.draw();
    
    control.drawStats();
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
    cout << "Exporting" << endl;
    control.exportPLY();
}

//--------------------------------------------------------------
void ofApp::addForceButtonPresssed(){
//    isMousePressed = !isMousePressed;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//system("say 'this is a quick test' &");  
}
