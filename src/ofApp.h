#pragma once

#include "ofMain.h"
#include "Control.h"
#include "ofxGui.h"
 
class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
    
        void drawAxis();
        void drawGrid();
        void sendOsc();
    
        void updateButtonPressed();
        void exportMeshButtonPressed();
        void addForceButtonPresssed();

		void keyPressed(int key);
		
        //Gui
        ofxPanel gui;
        //Booleans
        ofxToggle toggle_axis;
        ofxToggle toggle_grid;
        ofxToggle wire_frame;
    
        //Buttons
        ofxButton exportMeshButton;
        ofxButton addForceButton;
        //Sliders
        ofxFloatSlider param_A;
        ofxFloatSlider param_B;
        ofxFloatSlider param_C;

        ofLight light;
        ofEasyCam cam;

        Control control;
    
};
