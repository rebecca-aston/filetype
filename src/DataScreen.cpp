//
//  DataScreen.cpp
//  filetype
//
//  Created by Rebecca Aston on 8/12/18.
//

#include "DataScreen.h"

DataScreen::DataScreen(){
    colWidth = 400;
    imgSectionOrigin = ofVec2f(0,ofGetHeight()/2);
}

void DataScreen::setup(){
    font.load("GURU0.ttf",10);
    font.setLineHeight(font.getLineHeight()+3);
//    imgFbo->allocate(400,400,GL_RGBA);
}

void DataScreen::draw(){
    ofBackground(0);
    
    drawSection(imageFrame, 0, ofGetHeight()/2, colWidth, colWidth);

    drawSection(meshFrame, 0, 0, colWidth, colWidth);

    drawSection(textFrame, ofGetWidth()/2, 0, colWidth, colWidth);

    
}

void DataScreen::drawSection(frame f, float x, float y, float colW, float colH){
    
    float imgPadding = 0;
    float textPadding = 10;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x,y);
    
    ofPushStyle();
    ofSetColor(0);//Set to color to debug
    ofDrawRectangle(0,0,colW,colH);
    ofPopStyle();
    
    if(f.image.isAllocated()){
        scaleImageSquare(f.image, colW, imgPadding);
    }
    
    ofPushStyle();
    ofSetColor(0,0,255);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(0, 0, colWidth, colWidth);
    ofPopStyle();
    
    
//    if(font.isLoaded()){
//
//        int marginX = ofGetWidth()/2;
//        int lineY = ofGetHeight()/2;
//
//
//        font.drawString(wrapString(drawFrames[i].collection,colWidth),marginX,lineY);
//        lineY += font.stringHeight(wrapString(drawFrames[i].collection,colWidth))+6;
//        font.drawString(wrapString(drawFrames[i].title,colWidth), marginX, lineY);
//    }
    
    
    ofPopStyle();
    ofPopMatrix();
    
}

void DataScreen::scaleImageSquare(ofImage img, float size, float padding){
    ofPushMatrix();
    ofPushStyle();
    size = size-padding*2;
    ofTranslate(size/2,size/2);
    if(img.getWidth()>img.getHeight() || img.getWidth() == img.getHeight()){
        float scaleW = size/img.getWidth();
        ofScale(scaleW,scaleW);
    }else{
        float scaleH = size/img.getHeight();
        ofScale(scaleH,scaleH);
    }
    img.setAnchorPoint(img.getWidth()/2, img.getHeight()/2);
    img.draw(padding,padding);
    ofPopStyle();
    ofPopMatrix();
}

//https://forum.openframeworks.cc/t/text-wrapping/2953
string DataScreen::wrapString(string text, int width) {
    
    string typeWrapped = "";
    string tempString = "";
    vector <string> words = ofSplitString(text, " ");
    
    for(int i=0; i<words.size(); i++) {
        
        string wrd = words[i];
//        cout << wrd << endl;
        
        tempString += wrd + " ";
        int stringwidth = font.stringWidth(tempString);
        if(stringwidth >= width) {
            tempString = "";
            typeWrapped += "\n";
        }
        
        typeWrapped += wrd + " ";
    }
    
    return typeWrapped;
    
}


void DataScreen::drawImageRoulette(ofImage i){
    rouleteImage = i;
}

void DataScreen::loadData(frame current){
//    cout << current.frameType << endl;
//    cout << current.collection << endl;
//    removeFrame(current.frameType);
//    drawFrames.push_back(current);
    
//    Process frame?
    
    if(current.frameType == 2 || current.frameType == 4 ){
        meshFrame = current;
    }
    
    if(current.frameType == 3 ){
        imageFrame = current;
    }
    
    if(current.frameType == 5 ){
        textFrame = current;
    }
    
}

//void DataScreen::removeFrame(int t){
//    for(int i = 0; i < drawFrames.size();i++){
//        if(drawFrames[i].frameType == t) drawFrames.erase(drawFrames.begin()+i);
//    }
//}

