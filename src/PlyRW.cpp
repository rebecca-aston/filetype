//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "PlyRW.h"

PlyRW::PlyRW(){
    
}

Frame PlyRW::read(string s, int numParticles){
    ofxAssimpModelLoader model;
    Frame frame;
    
    model.loadModel(s);
    
    of3dPrimitive temp = model.getMesh(0);
    temp.setScale(5);// better to just set desired scale on export in Blender....
    
    for (int i = 0; i < temp.getMesh().getVertices().size(); i++){ 
        if(i%2==0){ //need to actually do the triangulation thing....
            frame.addPoint(temp.getMesh().getVertex(i));
            frame.addPointColor(temp.getMesh().getColor(i));
        }
    }
    
    //only works for me if I have one mesh, but I always do
    return frame;
}

//Adjust this logic for exporting to Blender/ply
//--------------------------------------------------------------
//void PlyRW::write(Frame frame, string fileName){
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
