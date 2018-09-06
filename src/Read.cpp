//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "Read.h"

Read::Read(){
    dev = false;
}

void Read::readModel(){
//    json
    modelData.open("json");
    
    if(modelData.exists()){
       
        modelData.listDir("json");
        modelData.getFiles();

        for(int i = 0; i < modelData.size(); i++){
            
            vector< frame > tempVec = readJson("json/"+modelData[i].getFileName());
            
            for(int j = 0; j < tempVec.size(); j++){
                
                if(tempVec[j].frameType == 1){//process commentary
                    processFrameVec.push_back(tempVec[j]);
                }
                if(tempVec[j].frameType == 2 || tempVec[j].frameType == 4){//Meshes and textures for 3D
                    threeDFrameVec.push_back(tempVec[j]);
                }
                if(tempVec[j].frameType == 3){//image
                    tempVec[j].totalTime = 500;
                    imageFrameVec.push_back(tempVec[j]);
                }
                if(tempVec[j].frameType == 5){//text
                    textFrameVec.push_back(tempVec[j]);
                }
                
            }
        
        }
    }

}

void Read::clearVectors(){
    imageFrameVec.clear();
    threeDFrameVec.clear();
    textFrameVec.clear();
    processFrameVec.clear();
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
            
            if(json["frames"][i]["frame-type"].asString() == "process"){
                temp.frameType = 1;
            }
            
            if(json["frames"][i]["frame-type"].asString() == "mesh"){
                temp.frameType = 2;
            }
            
            if(json["frames"][i]["frame-type"].asString() == "image"){
                temp.frameType = 3;
            }
            
            if(json["frames"][i]["frame-type"].asString() == "texture"){
                temp.frameType = 4;
            }
            
            if(json["frames"][i]["frame-type"].asString() == "text"){
                temp.frameType = 5;
            }
            
            if(dev)cout << json["frames"][i]["weight"].asFloat() << endl;
            temp.weight = json["frames"][i]["weight"].asFloat();
            
            for(int j = 0; j < json["frames"][i]["tags"].size(); j++){
                if(dev)cout << json["frames"][i]["tags"][j].asString() << endl;
                temp.tags.push_back(json["frames"][i]["tags"][j].asString());
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
                
                if((json["frames"][i]["history"][j]["text"].asString().size() < 1000 && json["frames"][i]["history"][j]["text"].asString().size() > 10 ) || json["frames"][i]["history"][j]["sound"].asString().size() > 0){
                    
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
                    
                    histEntry.sound = json["frames"][i]["history"][j]["sound"].asString();
                    
                    string text = json["frames"][i]["history"][j]["text"].asString();
                    
                    if(text.find(".txt") != std::string::npos && text.find("txt/") != std::string::npos ){ //check if it's a specific path to a text file
                        if(dev)cout << readText(text) << endl;
                        histEntry.text = readText(text);
                    }else{
                        if(dev)cout << text << endl;
                        histEntry.text = text;
                    }
                    
                    temp.historyVec.push_back(histEntry);
                    
                }
 
            }
            
            cout << temp.frameType << endl;
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
    
    txt = ofBufferFromFile(path).getText();
    if(dev) cout << txt << endl;
    
    return txt;
}

ofImage Read::readImage(string path){
    ofImage img;
    
    img.load(path);
    
    return img;
}


