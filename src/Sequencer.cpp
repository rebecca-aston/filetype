//
//  Sequencer.cpp
//  artifactlabEMMERSION
//
//  Created by Rebecca Aston on 7/17/18.
//

#include "Sequencer.h"

Sequencer::Sequencer(){
    
}

//Needs to be many current frames...

// a mesh or text/image with my own voice also just gets played
//History / read out stream -> so a frame with type text is also displaying any images.
//Images / Link is this weird middle thing which is adding images to particle (MAYBE NEED TO ABANDON)
//Mesh / texture / particles system stream


//Extend read to create separate framevecs by type

//Separate Sequencer out to be:
//sort on weights on any read framevec
//have multiple sequences....
//what about load frame...

//sequencer is a class that creates / stores sequence and takes in pointer to framevec
//sorts framevec
//manages sequence
//sequencer has a currentFrame that belongs to it
//it also stores what type it is

//Load frame is jsut being called on the vector of sequencers every update.
//Datascreen takes all current frames for all sequences and displays their data


//Commentary

//Have another field in frame that is commentary field.
//if populated points to the few sound files I have.


void Sequencer::setup(vector<frame> vecRef, string type){
    
    vecType = type;
    frameVec = vecRef; 
    
    //Store original order of data as unique identifier to be used for comparison later
    for(int i = 0; i < frameVec.size(); i++){
        frameVec[i].uID = ofToString(i)+vecType;
    }
    
    sequencer();
    
    //Initialize the sequence to one random frame from Model
//    sequence.push_back(&frameVec[ofRandom(frameVec.size())]);
    
//    cout << sequence.back()->uID << endl;
}

void Sequencer::update(){


}

void Sequencer::resetCurrentFrame(){
    
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
    
     for(int i = 0; i < frameVec.size(); i++){
         cout << frameVec[i].title ;
         cout << ": "+ofToString(frameVec[i].weight) << endl;
     }
    
    //Randomly choose from the first quarter of strongest weighted frames
    weightedRandom = ofRandom(ceil(frameVec.size()/2));
    if(frameVec[weightedRandom].uID == currentFrame.uID) weightedRandom ++; //leave in just in case get repetition
    
}


