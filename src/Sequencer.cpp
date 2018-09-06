//
//  Sequencer.cpp
//  artifactlabEMMERSION
//
//  Created by Rebecca Aston on 7/17/18.
//

#include "Sequencer.h"

Sequencer::Sequencer(){
    
}

void Sequencer::setup(vector<frame> vecRef, string type){
    
    vecType = type;
    frameVec = vecRef; 
    
    //Store original order of data as unique identifier to be used for comparison later
    for(int i = 0; i < frameVec.size(); i++){
        frameVec[i].uID = ofToString(i)+vecType;
    }
    
    sequencer();
    
}

void Sequencer::update(){


}

void Sequencer::resetCurrentFrame(){
    
    if(!forcedFrame){//If using weighted random choice load from vector
        currentFrame.uID = frameVec[weightedRandom].uID;
        currentFrame.frameType = frameVec[weightedRandom].frameType;
        currentFrame.weight = frameVec[weightedRandom].weight;
        currentFrame.tags = frameVec[weightedRandom].tags;
        currentFrame.totalTime = frameVec[weightedRandom].totalTime;
        currentFrame.externalFileName = frameVec[weightedRandom].externalFileName;
        
        currentFrame.collection = frameVec[weightedRandom].collection;
        currentFrame.collectionDesc = frameVec[weightedRandom].collectionDesc;
        //    int encounterDate;
        //    float encounterLocLong;
        //    float encounterLocLat;
        currentFrame.author = frameVec[weightedRandom].author;
        currentFrame.citation = frameVec[weightedRandom].citation;
        
        currentFrame.title = frameVec[weightedRandom].title;
        currentFrame.desc = frameVec[weightedRandom].desc;
        currentFrame.material = frameVec[weightedRandom].material;
        
        currentFrame.historyVec = frameVec[weightedRandom].historyVec;
        
        
        currentFrame.currentHistoryIndex = -1;
    }else{//else if using "curated" frame re-set boolean which is set when new frame forced
        forcedFrame = false;
    }
    
}

//Tis is used to add a "curated" proceudral frame into the sequence without adjusting the weights / sequencing of original data
void Sequencer::forceNewFrame(frame f, int fType){
    vector<string> tags;
    if(f.frameType == 1){ //Process / explosion / scatter (which has no tags)
        tags = currentFrame.tags;
    }

    //Boolean to skip the use of weighted random sequencer
    forcedFrame = true;
    
    currentFrame = f;
    currentFrame.frameType = fType;
    currentFrame.tags = tags;

    currentFrame.animating = true;
    
}

bool compareByWeight(const frame &a, const frame &b){
    return a.weight > b.weight;
}

bool in_array(const std::string &value, const std::vector<string> &array){
    return std::find(array.begin(), array.end(), value) != array.end();
}

//A sequencer of the content that is a weighted random choice based off of tags
//i.e. loose association to the current frame
void Sequencer::sequencer(){
    
    if(!forcedFrame){
        for(int i = 0; i < frameVec.size(); i++){
            float w = frameVec[i].weight;
            w *= 0.1;
            
            for(int j = 0; j < frameVec[i].tags.size(); j++){
                if(frameVec[i].uID != currentFrame.uID){
                    if(in_array(frameVec[i].tags[j],currentFrame.tags)){
                        
                        w += 1/float(currentFrame.tags.size());
                        
                    }
                }
            }
        
            frameVec[i].weight = MIN(w,1);

        }
        
        ofSort(frameVec, &compareByWeight);
        
        
        //Randomly choose from the first half of strongest weighted frames
        weightedRandom = ofRandom(floor(frameVec.size()/2));
        if(frameVec[weightedRandom].uID == currentFrame.uID) weightedRandom ++; //leave in just in case get repetition
//        frameVec[weightedRandom].weight = 0;//decrease chance of repetition?
    }
}


