//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "Write.h"

Write::Write(){
    
}



//Adjust this logic for exporting to Blender/ply
//--------------------------------------------------------------
void Write::writeMesh(frame f, string fileName) {

    ofMesh expMesh, expMeshB;

    //add all spheres to one mesh, with Indices update to reflect new vertex positions
    for(int i = 0; i < f.points.size(); i++){
        
        expMesh.addVertex(f.points[i]);
        expMesh.addColor(f.pointColors[i]);
        
//        expMesh.addIndex(i);

    }
    
    for(int i = 0; i < f.pointsB.size(); i++){
        
        expMeshB.addVertex(f.pointsB[i]);
        expMeshB.addColor(f.pointColorsB[i]);
        
        //        expMesh.addIndex(i);
        
    }

    expMesh.save(fileName+".ply");
    expMeshB.save(fileName+"_BackBurner.ply");
}


void Write::writeJson(frame f, string s, bool newFile){
    
    ofxJSONElement json;
    // e.g.
    //    json["light"]
    
    if(newFile){
        // frame write to new file
        
        vector< float > verts;
        
        for(int i = 0; i < f.points.size(); i++){
            verts.push_back(f.points[i].x);
            verts.push_back(f.points[i].y);
            verts.push_back(f.points[i].z);
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
