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
}

void DataScreen::draw(){
    if(font.isLoaded()){
        
        int centerScreen = ofGetWidth()/2;
        int newLine = ofGetHeight()/2;
        int lineHeight = font.stringHeight(currentFrame.collection);
        
        font.drawString(currentFrame.collection, centerScreen-(font.stringWidth(currentFrame.collection)/2), newLine);
        newLine += lineHeight;
        font.drawString(currentFrame.title, centerScreen-(font.stringWidth(currentFrame.title)/2), newLine);
        
    }
}

void DataScreen::loadData(frame current){
    cout << current.frameType << endl;
    cout << current.desc << endl;
    currentFrame = current;
}


