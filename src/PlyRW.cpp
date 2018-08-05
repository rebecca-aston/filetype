//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "PlyRW.h"

PlyRW::PlyRW(){
    
}

ofMesh PlyRW::read(string s, int numParticles){
    ofxAssimpModelLoader model;
    
    model.loadModel(s);
    
    ofMesh temp = model.getMesh(0);

    
//    for (int i = 0; i < temp.getMesh().getVertices().size(); i++){ 
//        if(i%2==0){ //need to actually do the triangulation thing....
//            frame1.points.push_back(temp.getMesh().getVertex(i));
//            frame1.pointColors.push_back(temp.getMesh().getColor(i));
//        }
//    }
    
    return temp;
}

//Adjust this logic for exporting to Blender/ply
//--------------------------------------------------------------
//void PlyRW::write(frame f, string fileName){
//
//    ofMesh expMesh;
//    int vertCount;
//
//    //add all spheres to one mesh, with Indices update to reflect new vertex positions
//    for(int i = 0; i < vertices.size(); i++){
//
//        vertices[i]->sphere.setRadius(vertices[i]->size*vertices[i]->scale);//Set radius to scale for export
//        ofVec3f pos = vertices[i]->sphere.getGlobalPosition();//get ofNode global position
//        ofMesh mesh = vertices[i]->sphere.getMesh();
//        for(int j = 0; j < mesh.getVertices().size();j++){
//            expMesh.addVertex(mesh.getVertex(j)+pos); //add global position to all vertex positions
//            expMesh.addColor(vertices[i]->color);
//            if(i==0) vertCount++;
//        }
//        for(int k = 0; k < mesh.getIndices().size();k++){
//            expMesh.addIndex(mesh.getIndex(k)+(vertCount*i));//update indices
//        }
//        vertices[i]->sphere.setRadius(vertices[i]->size); //revert particle in system back to size
//
//    }
//
//    expMesh.save(fileName);
//}

//Adjust this logic for exporting to Blender/ply
//--------------------------------------------------------------
void PlyRW::write(frame f, string fileName) {

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

