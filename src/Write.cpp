//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "Write.h"

Write::Write(){
    
}


//This is not fully executed
//Was exporting meshes from Control see PLY
//Will expand this in the future, it's more pseudo code / creative tools than central to the work

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

//moved from Control, need to write out properly
void Write::writePLY(){
//    frame temp;
    
    //for export it's not just the current frame, it's the whole system
    //move into JsonRW / equivalent name eventually
    
//    for(int i = 0; i < particleSystem.size(); i++){
//        if(particleSystem[i].x < (cubeResolution/2)-120){
//            ofVec3f p = ofVec3f(particleSystem[i].x,particleSystem[i].y,particleSystem[i].z);
//            ofVec3f p1 = ofVec3f(p.x + particleSystem[i].p1.x, p.y + particleSystem[i].p1.y, p.z + particleSystem[i].p1.z);
//            ofVec3f p2 = ofVec3f(p.x + particleSystem[i].p2.x, p.y + particleSystem[i].p2.y, p.z + particleSystem[i].p2.z);
//            if(p.distance(p1) < 30 && p.distance(p2) < 30){
//                temp.points.push_back(p);
//                temp.points.push_back(p1);
//                temp.points.push_back(p2);
//                temp.pointColors.push_back(particleSystem[i].targetColor);
//                temp.pointColors.push_back(particleSystem[i].p1Color);
//                temp.pointColors.push_back(particleSystem[i].p2Color);
//            }
//        }
//
//    }
    
    //    for(int i = 0; i < particleSystem.size(); i++){
    //        ofVec3f p = ofVec3f(particleSystem[i].x,particleSystem[i].y,particleSystem[i].z);
    //        ofVec3f p1 = ofVec3f(p.x + particleSystem[i].p1.x, p.y + particleSystem[i].p1.y, p.z + particleSystem[i].p1.z);
    //        ofVec3f p2 = ofVec3f(p.x + particleSystem[i].p2.x, p.y + particleSystem[i].p2.y, p.z + particleSystem[i].p2.z);
    //        if(p.distance(p1) < 30 && p.distance(p2) < 30){
    //            temp.points.push_back(p);
    //            temp.points.push_back(p1);
    //            temp.points.push_back(p2);
    //            temp.pointColors.push_back(particleSystem[i].targetColor);
    //            temp.pointColors.push_back(particleSystem[i].p1Color);
    //            temp.pointColors.push_back(particleSystem[i].p2Color);
    //        }
    //    }
    
    
    
    //    for(int i = 0; i < backBurnerSystem.size(); i++){
    //            ofVec3f p = ofVec3f(backBurnerSystem[i].x,backBurnerSystem[i].y,backBurnerSystem[i].z);
    //            ofVec3f p1 = ofVec3f(p.x + backBurnerSystem[i].p1.x, p.y + backBurnerSystem[i].p1.y, p.z + backBurnerSystem[i].p1.z);
    //            ofVec3f p2 = ofVec3f(p.x + backBurnerSystem[i].p2.x, p.y + backBurnerSystem[i].p2.y, p.z + backBurnerSystem[i].p2.z);
    //            if(p.distance(p1) < 30 && p.distance(p2) < 30){
    //                temp.pointsB.push_back(p);
    //                temp.pointsB.push_back(p1);
    //                temp.pointsB.push_back(p2);
    //                temp.pointColorsB.push_back(backBurnerSystem[i].targetColor);
    //                temp.pointColorsB.push_back(backBurnerSystem[i].p1Color);
    //                temp.pointColorsB.push_back(backBurnerSystem[i].p2Color);
    //            }
    //    }
    
//    write.writeMesh(temp,"pointCloud_"+ofGetTimestampString());
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
