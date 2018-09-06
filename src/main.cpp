#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    //Two screen set up with shared context
    
    ofGLFWWindowSettings settings;
    settings.width = 600;
    settings.height = 600;
    settings.setPosition(ofVec2f(300,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    settings.width = 1300;
    settings.height = 1027;
    settings.decorated = false;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
   
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setupSecondScreen();
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawSecondScreen);
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
    
}
