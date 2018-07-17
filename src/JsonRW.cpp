//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "JsonRW.h"

JsonRW::JsonRW(){
    
}

void JsonRW::read(string s){
    json.open(s);
}

//Weather data gathered by me
void JsonRW::write(Frame frame, string s, bool newFile){
    
    // e.g.
    //    json["light"]
    
    if(newFile){
        // frame write to new file
        
        vector< float > verts;
        
        for(int i = 0; i < frame.getPoints().size(); i++){
            verts.push_back(frame.getPoints()[i].x);
            verts.push_back(frame.getPoints()[i].y);
            verts.push_back(frame.getPoints()[i].z);
        }
        
        json["vertices"] = ofToString(verts);
        
        if( json.save(s+".json",true)){ //pretty json bool flag
            cout << "File saved successfully" << endl;
        }else{
            cout << "File failed to save" << endl;
        }
    }else{
        json.open(s+".json");
        //parse json and insert new frame information by either overwriting if field exists
        //or by adding new field if field does not exist
        
    }
    
}
