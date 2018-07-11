#include "MeshSystem.h"

////////////////////////
// SETUP SHAPE
//////////////////////
//--------------------------------------------------------------
MeshSystem::MeshSystem(){

}

//--------------------------------------------------------------
void MeshSystem::setup(int _initResolution, int _numParticles, string loadModel){ // ofVec3f
    
    if(loadModel == "none"){ //Randomized location in space
        initResolution = _initResolution;
        numParticles = _numParticles;
        
        for (int i = 0; i < numParticles; i++){
            ofVec3f pos = ofVec3f(ofRandom(-initResolution,initResolution), ofRandom(-initResolution,initResolution), ofRandom(-initResolution,initResolution));
            
            pushVert(pos,i,ofColor(255,255,255));
        }
    }else{//points begin where the model points were
        
        model.loadModel(loadModel);
        
        of3dPrimitive temp = model.getMesh(0);
        temp.setScale(5);
        
        for (int i = 0; i < temp.getMesh().getVertices().size(); i++){
            if(i%10==0){
                pushVert(temp.getMesh().getVertex(i),i,temp.getMesh().getColor(i));
            }
        
        }
        
    }
    
    cout << vertices.size() << endl;
    
}

//--------------------------------------------------------------
void MeshSystem::update(ofVec3f attractor) {
    
    for(int i = 0; i < vertices.size();i++){
        vertices[i]->updateVertex(attractor,&vertices,SIZE,PROBABILITY,SPEED,GRAVITY,SIDEWAYS,CHECKDIST);
    }
    
}

//--------------------------------------------------------------
void MeshSystem::draw(bool wireFrame){
    
    ofPushMatrix();
    
    for(int i = 0; i < vertices.size();i++){
        if(wireFrame){
            vertices[i]->sphere.drawWireframe();
        }else{
            vertices[i]->sphere.drawFaces();  
        }
    }
    
    ofPopMatrix();
}


//--------------------------------------------------------------
void MeshSystem::pushVert(ofVec3f pos, int i, ofColor _color){
    Vert * tempVert( new Vert(pos,i,_color) );
    vertices.push_back(tempVert);
}


//--------------------------------------------------------------
void MeshSystem::exportMesh(string fileName){

    ofMesh expMesh;
    int vertCount;
    
    //add all spheres to one mesh, with Indices update to reflect new vertex positions
    for(int i = 0; i < vertices.size(); i++){

            vertices[i]->sphere.setRadius(vertices[i]->size*vertices[i]->scale);//Set radius to scale for export
            ofVec3f pos = vertices[i]->sphere.getGlobalPosition();//get ofNode global position
            ofMesh mesh = vertices[i]->sphere.getMesh();
            for(int j = 0; j < mesh.getVertices().size();j++){
                expMesh.addVertex(mesh.getVertex(j)+pos); //add global position to all vertex positions
                expMesh.addColor(vertices[i]->color);
                if(i==0) vertCount++;
            }
            for(int k = 0; k < mesh.getIndices().size();k++){
                expMesh.addIndex(mesh.getIndex(k)+(vertCount*i));//update indices
            }
            vertices[i]->sphere.setRadius(vertices[i]->size); //revert particle in system back to size

    }
    
    expMesh.save(fileName);
}

