#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxDelaunay.h"
using namespace cv;

class DelaunayImage {

public:
    DelaunayImage();
    
    ofMesh triangulateImage(ofImage img);
	void draw();
    vector <ofPoint> getTriangle(int i);

	ofxCvColorImage image;

	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage inverted;
	ofxCvGrayscaleImage blurred;
	ofxCvGrayscaleImage mask;

    vector<Point2f> corners;
    
    ofxDelaunay triangulation;
};

