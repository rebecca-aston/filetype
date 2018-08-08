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


void Read::readModel(){
//    json
    modelData.open("json");
    
    if(modelData.exists()){
       
        modelData.listDir("json");
        modelData.getFiles();

        for(int i = 0; i < modelData.size(); i++){
            
            vector< frame > tempVec = readJson("json/"+modelData[i].getFileName());
            
            for(int j = 0; j < tempVec.size(); j++){
                
                frameVec.push_back(tempVec[j]);
                
            }
        
            //returned frame add to the larger data structure
        }
    }

}

vector< frame > Read::readJson(string path){
    ofxJSONElement json;
    vector< frame > vec;
    
    bool fileOpen = json.open(path);
    
    if(fileOpen){
        for(int i = 0; i < json["frames"].size(); i++){
            
            frame temp;
            
            //The meta-data of the larger collection
            if(dev)cout<< json["frame-collection"].asString() << endl;
            temp.collection = json["frame-collection"].asString();
            
            if(dev)cout<< json["description"].asString() << endl;
            temp.collectionDesc = json["description"].asString();
            
            if(dev)cout<< json["date-of-encounter"].asInt() << endl;
            temp.encounterDate = json["date-of-encounter"].asInt();
            
            if(dev)cout<< json["encounter-loc-long"].asFloat() << endl;
            temp.encounterLocLong = json["encounter-loc-long"].asFloat();
            
            if(dev)cout<< json["encounter-loc-lat"].asFloat() << endl;
            temp.encounterLocLat = json["encounter-loc-lat"].asFloat();
            
            if(dev)cout<< json["author"].asString() << endl;
            temp.author = json["author"].asString();
            
            if(dev)cout<< json["citation"].asString() << endl;
            temp.citation = json["citation"].asString();
            
            
            //Frame specific data
            if(dev)cout << json["frames"][i]["frame-type"].asString() << endl;
            if(json["frames"][i]["frame-type"].asString() == "mesh"){
                temp.frameType = 2;
            }
            
            if(dev)cout << json["frames"][i]["title"].asString() << endl;
            temp.title = json["frames"][i]["title"].asString();
            
            if(dev)cout << json["frames"][i]["description"].asString() << endl;
            temp.desc = json["frames"][i]["description"].asString();
            
            if(dev)cout << json["frames"][i]["material"].asString() << endl;
            temp.material = json["frames"][i]["material"].asString();
            
            if(dev)cout << json["frames"][i]["file-name"].asString() << endl;
            temp.externalFileName = json["frames"][i]["file-name"].asString();
            
            if(dev)cout<< json["frames"][i]["total-time"].asInt() << endl;
            temp.totalTime = json["frames"][i]["total-time"].asInt();
            
            for(int j = 0; j < json["frames"][i]["history"].size(); j++){
                history histEntry;
                
                if(dev)cout << json["frames"][i]["history"][j]["speculation-scale"].asString() << endl;
                histEntry.speculativeScale = json["frames"][i]["history"][j]["speculation-scale"].asString();
                
                if(dev)cout << json["frames"][i]["history"][j]["date"].asInt() << endl;
                histEntry.date = json["frames"][i]["history"][j]["date"].asInt();
                
                if(dev)cout << json["frames"][i]["history"][j]["loc-long"].asFloat() << endl;
                histEntry.locLong = json["frames"][i]["history"][j]["loc-long"].asFloat();
                
                if(dev)cout << json["frames"][i]["history"][j]["loc-lat"].asFloat() << endl;
                histEntry.locLat = json["frames"][i]["history"][j]["loc-lat"].asFloat();
                
                if(dev)cout << json["frames"][i]["history"][j]["citation"].asString() << endl;
                histEntry.citation = json["frames"][i]["history"][j]["citation"].asString();
                
                string text = json["frames"][i]["history"][j]["text"].asString();
                
                if(text.find(".txt") == true && text.find("txt/") == true ){ //check if it's a specific path to a text file
                    if(dev)cout << readText(text) << endl;
                    histEntry.text = json["frames"][i]["history"][j]["citation"].asString();
                }else{
                    if(dev)cout << text << endl;
                    histEntry.text = json["frames"][i]["history"][j]["citation"].asString();
                }
                
                temp.historyVec.push_back(histEntry);
            }
            
            vec.push_back(temp);
        }
        
    }
    
    
    return vec;
}

ofMesh Read::readMesh(string path){
    ofxAssimpModelLoader model;
    
    model.loadModel(path);
    
    ofMesh temp = model.getMesh(0);
    
    
    return temp;
}

string Read::readText(string path){
    string txt;
    
//    test this later
//    txt = ofBufferFromFile(path).getText();
    cout << "READTEXT NOT IMPLEMENTED YET" << endl;
    
    return txt;
}



