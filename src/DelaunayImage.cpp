#include "DelaunayImage.h"

DelaunayImage::DelaunayImage(){
    addTextureToSystem = false;
}

//--------------------------------------------------------------
ofMesh DelaunayImage::triangulateImage(ofImage img){
    
        ofMesh temp;
    
        img.mirror(true, false);
    
        image.setFromPixels( img.getPixels() );
    

		//Convert to grayscale image
		grayImage = image;

		//Smoothing image
		blurred = grayImage;
		blurred.blurGaussian( 20);
    

        //#### FIND INTERESTING POINTS #####
		Mat imageCV;
		imageCV = Mat( blurred.getCvImage() );
		goodFeaturesToTrack(imageCV, corners, 1000, 0.01, 4); //param 3 is how many points you want to get
        
        
        triangulation.reset();
    
        for(int i = 0; i < corners.size(); i++){
            triangulation.addPoint(ofPoint(corners[i].x, corners[i].y));
        }
        triangulation.triangulate(); // calculate the triangulation!
    
    
        //Increment so that don't add incorrect indices using i in for loop
        int numTri = 0;
    
        for (int i=0; i<triangulation.getNumTriangles(); i++){ // loop over the triangles
            vector <ofPoint> pts = getTriangle(i);             // extract the vector with 3 points
            int cX = MAX(0,MIN(image.getWidth(),(pts[0].x+pts[1].x+pts[2].x)/3));
            int cY = MAX(0,MIN(image.getHeight(),(pts[0].y+pts[1].y+pts[2].y)/3));
            ofPoint centroid = ofPoint(cX,cY);

//            Remove black
            if(image.getPixels().getColor(centroid.x,centroid.y).r != 0 && image.getPixels().getColor(centroid.x,centroid.y).g != 0 && image.getPixels().getColor(centroid.x,centroid.y).b != 0){

                int randY = ofRandom(0,30);

                temp.addVertex(ofVec3f(pts[0].x,randY,pts[0].y));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(numTri*3);

                temp.addVertex(ofVec3f(pts[1].x,randY,pts[1].y));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(numTri*3+1);

                temp.addVertex(ofVec3f(pts[2].x,randY,pts[2].y));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(numTri*3+2);
                
                numTri ++;
            }

        }
    
    return temp;

}

//--------------------------------------------------------------
// custom function that takes an index and returns the coordinates of the triangle we refer to
vector <ofPoint> DelaunayImage::getTriangle(int i){
    int pA = triangulation.triangleMesh.getIndex(i*3);
    int pB = triangulation.triangleMesh.getIndex(i*3+1);
    int pC = triangulation.triangleMesh.getIndex(i*3+2);
    
    ofPoint pointA = triangulation.triangleMesh.getVertex(pA);
    ofPoint pointB = triangulation.triangleMesh.getVertex(pB);
    ofPoint pointC = triangulation.triangleMesh.getVertex(pC);
    
    vector <ofPoint> points;
    points.push_back(pointA);
    points.push_back(pointB);
    points.push_back(pointC);
    return points;
}

void DelaunayImage::setupRoulete(){
    
    imgFbo.allocate(400,400,GL_RGB);
    
    imageDirectory.open("images");
    
    if(imageDirectory.exists()){
        
        imageDirectory.listDir("images");
        imageDirectory.getFiles();
        
    }
    
}

void DelaunayImage::processImage(){
    
    ofImage img;
    
    addTextureToSystem = (ofRandom(1)>.9)?true:false;
    
    string file = imageDirectory[ofRandom(imageDirectory.size())].getFileName();//
    
    cout << file << endl;
    
    img.load("images/"+file);
    
    imgFbo.begin();
    
    ofClear(0,0,0);
    
    if(img.getWidth()>img.getHeight()){
        ofPushMatrix();
        float scale = imgFbo.getWidth()/img.getWidth();
        ofScale(scale,scale);
        img.draw(0, imgFbo.getHeight()-img.getHeight()/2);
        ofPopMatrix();
    }else{
        ofPushMatrix();
        float scale = imgFbo.getHeight()/img.getHeight();
        ofScale(scale,scale);
        img.draw(imgFbo.getWidth()-img.getWidth()/2,0);
        ofPopMatrix();
    }
    
    if(addTextureToSystem){
        //Do logic to isolate image before returning fbo
        
        //Here set the fbo to an image value
        //Then Process/isolate
        //Then delaunay it
        //Store mesh as entry on this object to be used in next step to create frame
        
        //Add weird process overlay to FBO to indicate that it has been chosen
    }
    
    imgFbo.end();
    
}



