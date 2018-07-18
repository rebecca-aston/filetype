//
//  Frame.cpp
//  artifactlabEMMERSION
//
//  Created by Rebecca Aston on 7/17/18.
//

#include "Frame.h"

Frame::Frame(){

}

//This might get too bulky copying things, might have to do pointers to the original data....

string Frame::getType(){
    return frameType;
}

void Frame::setType(string t){
    frameType = t;
}

vector< ofVec3f > Frame::getPoints(){
    return points;
}

void Frame::setPoints(vector< ofVec3f > c){
    points = c;
}

void Frame::addPoint(ofVec3f p){
    points.push_back(p);
}

vector< ofColor > Frame::getPointColors(){
    return pointColors;
}

void Frame::setPointColors(vector< ofColor > p){
    pointColors = p;
}

void Frame::addPointColor(ofColor c){
    pointColors.push_back(c);
}




