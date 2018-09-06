//
//  DataScreen.cpp
//  filetype
//
//  Created by Rebecca Aston on 8/12/18.
//

#include "DataScreen.h"

//This all got a bit messy with hard coded pixel changes to make it fit perfectly to
//the installed screen in my table, but for making pruposes you can get an idea

DataScreen::DataScreen(){
    colWidth = ofGetWidth()/2-20;
    
    imgPadding = 0;
    lineHeight = 6;
    textPadding = 13;
}

void DataScreen::setup(){
    font.load("GURU0.ttf",9);
    font.setLineHeight(font.getLineHeight()+5);
    
    camBirdsEye.setGlobalPosition(0, 500, 2000);
    camBirdsEye.lookAt(ofVec3f(0, 500, 0));

}

void DataScreen::draw(BinnedParticleSystem * p, BinnedParticleSystem * b){
    ofBackground(0);
    
    camBirdsEye.begin();
    b->draw();
    p->draw();
    camBirdsEye.end();
    
    ofPushStyle();
    ofSetColor(200);
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(colWidth+10, 0, colWidth+2, ofGetHeight()-3);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(0);
    ofDrawRectangle(10, 0, colWidth, ofGetHeight());
    ofPopStyle();
    
    drawSection(textFrame, 10, 0, colWidth, colWidth);

    drawSection(meshFrame, 10, ofGetHeight()-colWidth*1.12, colWidth*0.6, colWidth); //EDIT HERE

    drawSection(imageFrame, colWidth*0.6+10, ofGetHeight()-colWidth*1.12, colWidth*0.4, colWidth); //EDIT HERE


    updateMeta();
}

void DataScreen::drawSection(frame f, float x, float y, float colW, float colH){
    
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x,y);
    
    ofPushStyle();
    ofSetColor(0);//Set to color to debug
    ofDrawRectangle(0,0,colW,colH);
    ofPopStyle();
    
    if(f.image.isAllocated() && f.frameType != 5){ //frametype text
        scaleImageSquare(f.image, colW, imgPadding);
    }
    
    ofPushStyle();
    ofSetColor(200);
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, colW, colH);
    ofPopStyle();
    
    
    if(font.isLoaded()){
        
        if(f.frameType == 5){
            textLayout(f,0,0,colW,colH);
        }else{
            textLayout(f,0,colW,colW,colH);
        }
        
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}

void DataScreen::textLayout(frame f, float x, float y, float colW, float colH){
    int totalW = colW-textPadding*2;
    int marginX = x+textPadding;
    int lineY = y+textPadding*2;
    
    
    font.drawString(wrapString(f.title,totalW),marginX,lineY);
    lineY += font.stringHeight(wrapString(f.title,totalW))+lineHeight;
    font.drawString(wrapString(f.desc,totalW), marginX, lineY);
    lineY += font.stringHeight(wrapString(f.desc,totalW))+lineHeight;

    if(f.author != ""){
        string author = wrapString("Author: "+ f.author,totalW);
        font.drawString(author, marginX, lineY);
        lineY += font.stringHeight(author)+lineHeight;
    }
    
    if(f.frameType == 5 && f.historyVec.size() > 0){ // slightly hacky as I just ended up using one "history" item per frame
        font.drawString(wrapString(f.historyVec[0].text,totalW-100),marginX, lineY);
    }
    
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

void DataScreen::updateMeta(){
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(0,ofGetHeight()*0.77); //EDIT FOR PLACEMENT HERE

    ofSetColor(0);
    ofDrawRectangle(10,0,ofGetWidth()-38,60);
    
    ofSetColor(255);
    
    string frameName = "New Data: ";
    
    font.drawString(frameName+newestFrame.title,textPadding+10,textPadding+3);
    
    string tags = "Tags: ";
    for(int i = 0; i < newestFrame.tags.size(); i++){
        tags.append(newestFrame.tags[i]);
        if(i != newestFrame.tags.size()-1){
            tags.append(", ");
        }
    }
    font.drawString(tags,textPadding+10,33);
    
    if(newestFrame.citation != ""){
        string citation = "Citation: ";
        font.drawString(citation+newestFrame.citation,textPadding+10,50);
    }
    
    ofPushStyle();
    ofSetColor(200);
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(10, 0, ofGetWidth()-38,60);
    ofPopStyle();
    
    ofPopStyle();
    ofPopMatrix();
}

void DataScreen::loadData(frame current){
    
    if(current.frameType == 2 || current.frameType == 4 ){
        meshFrame = current;
    }
    
    if(current.frameType == 3 ){
        imageFrame = current;
    }
    
    if(current.frameType == 5 ){
        textFrame = current;
    }
    
    if(current.frameType != 1 && current.frameType != 3 ){ //exploding frame has no meta-data

        newestFrame = current;
    }
    if(current.frameType == 3 && current.addToMeta){ // Make a stronger visual correlation with image randomly add to particle system
        newestFrame = current;
    }
}

