//
//  DataScreen.cpp
//  filetype
//
//  Created by Rebecca Aston on 8/12/18.
//

#include "DataScreen.h"

DataScreen::DataScreen(){
    colWidth = ofGetWidth()/2-20;
    
    imgPadding = 0;
    lineHeight = 6;
    textPadding = 13;
}

// Set camera and just call draw particle system in it again...
//see if that's too much for the system....

void DataScreen::setup(){
    font.load("GURU0.ttf",9);
    font.setLineHeight(font.getLineHeight()+5);
//    imgFbo->allocate(400,400,GL_RGBA);
    
    camBirdsEye.setGlobalPosition(0, 300, 800);
    camBirdsEye.lookAt(ofVec3f(0, 300, 0));

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

    drawSection(meshFrame, 10, ofGetHeight()-colWidth, colWidth*0.67, colWidth*0.67);

    drawSection(imageFrame, colWidth*0.67+10, ofGetHeight()-colWidth, colWidth*0.33, colWidth*0.33);

    
//    drawMeta() function here
    
    
}

void DataScreen::drawSection(frame f, float x, float y, float colW, float colH){
    
    
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
    lineY += font.stringHeight(wrapString(f.collection,totalW))+lineHeight;
    font.drawString(wrapString(f.desc,totalW), marginX, lineY);
    lineY += font.stringHeight(wrapString(f.desc,totalW))+lineHeight;

    if(f.author != ""){
        string author = wrapString("Author: "+ f.author,totalW);
        font.drawString(author, marginX, lineY);
        lineY += font.stringHeight(author)+lineHeight;
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
    
//    vector <string> splitWord;
//    int count = 0;
//    while(count<words.size()) {
//        string wrd = words[count];
//        ofDrawBitmapString(ofToString(font.stringWidth(wrd)) + " length", ofGetWidth()/2+32, ofGetHeight()/2);
//        if(font.stringWidth(wrd) >= (width-10)){
//            int avgChar = font.stringWidth(wrd)/wrd.size();
//            int maxChar = floor((width-10)/avgChar);
////            string str =
//            splitWord.push_back(wrd.substr(0,maxChar));
//            if(wrd.substr(maxChar+1,wrd.size()-1).size()>0){
//                words.insert(words.begin()+(count+1),wrd.substr(maxChar+1,wrd.size()-1));
//                count ++;
//            }
//        }else{
//            if(count< words.size()){
//                count ++;
//            }else{
//                break;
//            }
//        }
//    }
    
//    if(splitWord.size()>0){
//        words = splitWord;
//    }
    
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

void DataScreen::updateMeta(frame f){
    
    //Set variable to highlight border around "active" frame
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(0,0);
    
    ofSetColor(0);
    ofDrawRectangle(0,0,ofGetWidth(),100);
    
//    if(f.citation != ""){
//        string citation = wrapString("Citation: "+ f.citation,totalW);
//        font.drawString(citation,marginX,lineY);
//    }
    
    string tags = "Tags: ";
    for(int i = 0; i < f.tags.size(); i++){
        tags.append(f.tags[i]);
        if(i == f.tags.size()-1){
            tags.append(", ");
        }
    }
    font.drawString(tags,0,0);
    
    ofPushStyle();
    ofSetColor(200);
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, ofGetWidth(),100);
    ofPopStyle();
    
    ofPopStyle();
    ofPopMatrix();
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
    
    
    //Push all this into variable in this function
//    updateMeta(current);
    
}

//void DataScreen::removeFrame(int t){
//    for(int i = 0; i < drawFrames.size();i++){
//        if(drawFrames[i].frameType == t) drawFrames.erase(drawFrames.begin()+i);
//    }
//}

