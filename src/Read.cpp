//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "Read.h"

Read::Read(){
    
}


//I think definitely store a reference of what is in the system
//Keep the JSON files really light
//SO keep a very loose amount of

//Add the collection data to the "meta-data" of each frame

// on load / setup
// read in all JSON diles in the json directory into data object
// is it an array, is it an std object map thing?

//then in controller... on frame load get all the associated data loaded in to the current frame
//maybe change it a bit, like if it's not a new frame and the deque is just looping
//then don't "load frame"
//no there should be a load data that gets called once when new frame is added to the deque (loadData in Read)
//then in control it should be play next instead of load frame....

//loadData(string type, string filename) return
//hmmm no
// got to be
// readImage
// readSound
// readText
// readMesh



frame Read::readJson(string s){
    ofxJSONElement json;
    frame temp;
    json.open(s);
    
    return temp;
}

ofMesh Read::readMesh(string s){ 
    ofxAssimpModelLoader model;
    
    model.loadModel(s);
    
    ofMesh temp = model.getMesh(0);
    
    
    return temp;
}




