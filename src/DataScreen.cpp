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
    
    for(int i = 0; i < drawFrames.size();i ++){
        
        if(font.isLoaded()){
            
            int marginX = ofGetWidth()/2;
            int lineY = ofGetHeight()/2;
            
//            font.drawString(drawFrames[i].collection, centerScreen-(font.stringWidth(drawFrames[i].collection)/2), newLine);
//            newLine += lineHeight;

            
            font.drawString(wrapString(drawFrames[i].collection,colWidth),marginX,lineY);
            lineY += font.stringHeight(wrapString(drawFrames[i].collection,colWidth))+6;
            font.drawString(wrapString(drawFrames[i].title,colWidth), marginX, lineY);
        

            switch(drawFrames[i].frameType){
                case 1 : { // Process


                    break;
                }
                case 2 : { // Mesh
                    drawFrames[i].mesh.drawFaces();
                    cout << "HERE" << endl;
                    break;
                }
                case 3 : { // image

                    //Special Roulete case
    //                if(currentFrame.image.isAllocated()){
    //                    ofPushMatrix();
    //                    //            ofScale(.5, .5);
    //                    currentFrame.image.draw(0,0);
    //                    ofPopMatrix();
    //                }



                    break;
                }
                case 4 : { // Texture
                    if(drawFrames[i].image.isAllocated()){
                        ofPushMatrix();
                        //            ofScale(.5, .5);
                        drawFrames[i].image.draw(0,0);
                        ofPopMatrix();
                    }

                    break;
                }
                case 5 : { // Text
                    if(drawFrames[i].image.isAllocated()){
                        ofPushMatrix();
                        //            ofScale(.5, .5);
                        drawFrames[i].image.draw(ofGetWidth()/2,0);
                        ofPopMatrix();
                    }

                    break;
                }
            }
        }
    }
    
    if(rouleteImage.isAllocated()){
        //scale it correctly
        float scaleW = colWidth/rouleteImage.getWidth();
        float scaleH = colWidth/rouleteImage.getHeight();
        
        ofPushMatrix();
        
        ofTranslate(imgSectionOrigin.x, imgSectionOrigin.y);
        
        if(rouleteImage.getWidth()>rouleteImage.getHeight()){
            ofPushMatrix();
            ofScale(scaleW,scaleW);
//            rouleteImage.draw(imgSectionOrigin.x, imgSectionOrigin.y+(colWidth-(rouleteImage.getHeight()*scaleW))/2);
            rouleteImage.draw(0, (colWidth-rouleteImage.getHeight()*scaleW)/2);
            ofPopMatrix();
        }else{
            ofPushMatrix();
            ofScale(scaleH,scaleH);
//            rouleteImage.draw(imgSectionOrigin.x+(colWidth-(rouleteImage.getWidth()*scaleH))/2,imgSectionOrigin.y );
            rouleteImage.draw((colWidth-rouleteImage.getWidth()*scaleH)/2,0);
            ofPopMatrix();
        }
 
        
        ofSetColor(0,0,255);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawRectangle(0, 0, colWidth, colWidth);
        ofPopMatrix();
        
    }
    
}

//https://forum.openframeworks.cc/t/text-wrapping/2953
string DataScreen::wrapString(string text, int width) {
    
    string typeWrapped = "";
    string tempString = "";
    vector <string> words = ofSplitString(text, " ");
    
    for(int i=0; i<words.size(); i++) {
        
        string wrd = words[i];
        cout << wrd << endl;
        
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
    cout << current.frameType << endl;
    cout << current.collection << endl;
    removeFrame(current.frameType);
    drawFrames.push_back(current);
}

void DataScreen::removeFrame(int t){
    for(int i = 0; i < drawFrames.size();i++){
        if(drawFrames[i].frameType == t) drawFrames.erase(drawFrames.begin()+i);
    }
}

