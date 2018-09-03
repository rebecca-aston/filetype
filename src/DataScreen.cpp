//
//  DataScreen.cpp
//  filetype
//
//  Created by Rebecca Aston on 8/12/18.
//

#include "DataScreen.h"

DataScreen::DataScreen(){
    
}

void DataScreen::setup(){
    font.load("Calibri.ttf",15);
//    imgFbo->allocate(400,400,GL_RGBA);
}

void DataScreen::draw(){
    
    ofPushMatrix();
    ofBackground(0);
    ofPopMatrix();
    if(font.isLoaded()){
        


        if(currentFrame.image.isAllocated()){
            ofPushMatrix();
//            ofScale(.5, .5);
            currentFrame.image.draw(0,0);
            ofPopMatrix();
        }
        
        if(rouleteImage.isAllocated()){
            //scale it correctly
            rouleteImage.draw(0,ofGetHeight()/2);///-imgFbo.getWidth()
        }
    
        //Only responsible for dispalying on second screen
        //So whatever updates to the historyvec are happening
        //they are just being shown here.
        
        
        
        
        int centerScreen = ofGetWidth()/2;
        int newLine = ofGetHeight()/2;
        int lineHeight = font.stringHeight(currentFrame.collection);
        
        font.drawString(currentFrame.collection, centerScreen-(font.stringWidth(currentFrame.collection)/2), newLine);
        newLine += lineHeight;
        font.drawString(currentFrame.title, centerScreen-(font.stringWidth(currentFrame.title)/2), newLine);
        
    }
}

void DataScreen::drawImageRoulette(ofImage i){
    rouleteImage = i;
}

void DataScreen::loadData(frame current){
//    cout << current.frameType << endl;
//    cout << current.desc << endl;
    currentFrame = current;
}


