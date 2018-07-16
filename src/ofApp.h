#pragma once

#include "ofMain.h"
#include "MeshSystem.h"
#include "BinnedParticleSystem.h"
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
        MeshSystem meshSystem;
    
    
    
    
    // Binned particle setup
    
    float timeStep;
    float particleNeighborhood, particleRepulsion, particleCohesion, particleAlign; //Hmm align is probably more akin to directional vector
    float centerAttraction;
    float cubeResolution;
    
    int kBinnedParticles;
    BinnedParticleSystem particleSystem;
    bool isMousePressed, slowMotion;
    
    bool drawBalls;
    
    
    
};
