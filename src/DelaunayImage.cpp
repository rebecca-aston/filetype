#include "DelaunayImage.h"

DelaunayImage::DelaunayImage(){
    
}

//--------------------------------------------------------------
ofMesh DelaunayImage::triangulateImage(ofImage img){
    
        ofMesh temp;
    
        image.setFromPixels( img.getPixels() );
    

		//Convert to grayscale image
		grayImage = image;

		//Smoothing image
		blurred = grayImage;
		blurred.blurGaussian( 20);
    
        

//		//Thresholding for obtaining binary image
//		mask = blurred;
//		mask.threshold( ofMap(mouseX, 0, ofGetWidth(), 0, 200) );

//        //Getting an inverted mask of image to use in contour finder
//        inverted = mask;
//        inverted.invert();

        //#### FIND INTERESTING POINTS #####
		Mat imageCV;
		imageCV = Mat( blurred.getCvImage() );
		goodFeaturesToTrack(imageCV, corners, 1000, 0.01, 4); //param 3 is how many points you want to get
        
        
        triangulation.reset();
    
        for(int i = 0; i < corners.size(); i++){
            triangulation.addPoint(ofPoint(corners[i].x, corners[i].y));
        }
        triangulation.triangulate(); // calculate the triangulation!
    
    
        for (int i=0; i<triangulation.getNumTriangles(); i++){ // loop over the triangles
            vector <ofPoint> pts = getTriangle(i);             // extract the vector with 3 points
            ofPoint centroid = ofPoint((pts[0].x+pts[1].x+pts[2].x)/3,(pts[0].y+pts[1].y+pts[2].y)/3);
            
            
            
            
//            Take out the black i.e. if black don't add

            
            if(image.getPixels().getColor(centroid.x,centroid.y).r != 0 && image.getPixels().getColor(centroid.x,centroid.y).g != 0 && image.getPixels().getColor(centroid.x,centroid.y).b != 0){
                
                temp.addVertex(ofVec3f(pts[0].x,pts[0].y,ofRandom(0,50)));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(i*3);
                
                temp.addVertex(ofVec3f(pts[1].x,pts[1].y,ofRandom(0,50)));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(i*3+1);
                
                temp.addVertex(ofVec3f(pts[2].x,pts[2].y,ofRandom(0,50)));
                temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                temp.addIndex(i*3+2);
                
            }
            
            
 
            
//            if(pts[0].distance(pts[1]) < 130.0 && pts[1].distance(pts[2]) < 130.0 && pts[2].distance(pts[0]) < 130.0){
//
//            }else{
//
//            }
            
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
