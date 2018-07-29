//
//  Model.cpp
//  3dArtefactLab
//
//  Created by Rebecca Aston on 3/6/18.
//

#include "PlyRW.h"

PlyRW::PlyRW(){
    
}

frame PlyRW::read(string s, int numParticles){
    ofxAssimpModelLoader model;
    frame frame1;
    
    //just for testing
    
    frame1.totalTime = 20000;
    frame1.frameType = 2;
    
    model.loadModel(s);
    
    of3dPrimitive temp = model.getMesh(0);
    temp.setScale(5);// better to just set desired scale on export in Blender....
    
//    for (int i = 0; i < temp.getMesh().getVertices().size(); i++){ 
//        if(i%2==0){ //need to actually do the triangulation thing....
//            frame1.points.push_back(temp.getMesh().getVertex(i));
//            frame1.pointColors.push_back(temp.getMesh().getColor(i));
//        }
//    }
    
    //temporary
    int count = 0;
    
    //So here we are just taking all the points of the mesh (up until a certain size/amount perhaps) and then only when instantiating the particle system do we do the calculation of the center etc.
    
    for (int i = 0; i < temp.getMesh().getIndices().size(); i+=3){
        if(count < numParticles){ //need to actually do the triangulation thing....
            frame1.points.push_back(temp.getMesh().getVertex(temp.getMesh().getIndex(i)));
            frame1.pointColors.push_back(temp.getMesh().getColor(temp.getMesh().getIndex(i)));
            
            frame1.points.push_back(temp.getMesh().getVertex(temp.getMesh().getIndex(i+1)));
            frame1.pointColors.push_back(temp.getMesh().getColor(temp.getMesh().getIndex(i+1)));
            
            frame1.points.push_back(temp.getMesh().getVertex(temp.getMesh().getIndex(i+2)));
            frame1.pointColors.push_back(temp.getMesh().getColor(temp.getMesh().getIndex(i+2)));
            
            
            frame1.pointColors.push_back(temp.getMesh().getColor(i));
            frame1.pointColors.push_back(temp.getMesh().getColor(i+1));
            frame1.pointColors.push_back(temp.getMesh().getColor(i+2));
            count ++;
        }
    }
    
    //only works for me if I have one mesh, but I always do
    return frame1;
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
