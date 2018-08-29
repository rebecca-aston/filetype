#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxGui.h"
 
class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    
        void setupSecondScreen();
//        void update();
        void drawSecondScreen(ofEventArgs & args);
    
        void drawAxis();
        void drawGrid();
        void sendOsc();
    
        void exportMeshButtonPressed();
        void addForceButtonPresssed();

		void keyPressed(int key);
		
        //Gui
        ofxPanel gui;
        //Booleans
        ofxToggle toggle_axis;
        ofxToggle toggle_grid;
        ofxToggle pause;
        ofxToggle presentationMode;
    
    
        //Buttons
        ofxButton exportMeshButton;

        //Sliders
        ofxFloatSlider param_A;
        ofxFloatSlider param_B;
        ofxFloatSlider param_C;

        ofLight light;
        ofEasyCam cam;

        Control control;
    
};
