#include "DelaunayImage.h"

//Taken from on of out class exercises and expanded a little

DelaunayImage::DelaunayImage(){

}

//IsVertical sets the orientation of the "mesh" / Plane in space
ofMesh DelaunayImage::triangulateImage(ofImage img, bool isVertical, float cubeResolution){
    
        ofMesh temp;
    
        img.mirror(true, false);
    
        image.setFromPixels( img.getPixels() );
    

		//Convert to grayscale image
		grayImage = image;

		//Smoothing image
		blurred = grayImage;
		blurred.blurGaussian( 20);
    
		Mat imageCV;
		imageCV = Mat( blurred.getCvImage() );
		goodFeaturesToTrack(imageCV, corners, 1000, 0.01, 4); //param 3 is how many points you want to get
        
        
        triangulation.reset();
    
        for(int i = 0; i < corners.size(); i++){
            triangulation.addPoint(ofPoint(corners[i].x, corners[i].y));
        }
        triangulation.triangulate();
    
    
    
        int numTri = 0;//Increment so that don't add incorrect indices using i in for loop
        ofVec3f newOrigin = ofVec3f(ofRandom(cubeResolution*0.25,cubeResolution*0.75),ofRandom(cubeResolution*0.25,cubeResolution*0.75),ofRandom(0,cubeResolution/4));
    
        for (int i=0; i<triangulation.getNumTriangles(); i++){ // loop over the triangles
            vector <ofPoint> pts = getTriangle(i);             // extract the vector with 3 points
            int cX = MAX(0,MIN(image.getWidth(),(pts[0].x+pts[1].x+pts[2].x)/3));
            int cY = MAX(0,MIN(image.getHeight(),(pts[0].y+pts[1].y+pts[2].y)/3));
            ofPoint centroid = ofPoint(cX,cY);

//          Remove black bits as the backgrouns is black
            if(image.getPixels().getColor(centroid.x,centroid.y).r != 0 && image.getPixels().getColor(centroid.x,centroid.y).g != 0 && image.getPixels().getColor(centroid.x,centroid.y).b != 0){

                int rand = ofRandom(0,30);
                
                if(isVertical){
                    temp.addVertex(ofVec3f(pts[0].x,rand,pts[0].y)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3);
                    
                    temp.addVertex(ofVec3f(pts[1].x,rand,pts[1].y)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3+1);
                    
                    temp.addVertex(ofVec3f(pts[2].x,rand,pts[2].y)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3+2);
                }else{
                    temp.addVertex(ofVec3f(pts[0].x,pts[0].y,rand)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3);
                    
                    temp.addVertex(ofVec3f(pts[1].x,pts[1].y,rand)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3+1);
                    
                    temp.addVertex(ofVec3f(pts[2].x,pts[2].y,rand)+newOrigin);
                    temp.addColor(image.getPixels().getColor(centroid.x,centroid.y));
                    temp.addIndex(numTri*3+2);
                }

                numTri ++;
            }

        }
    
    return temp;

}

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
