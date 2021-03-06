#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetVerticalSync(true);
    //cam.setVerbose(true);
    cam.setDeviceID(1); // for multiple device cameras
    cam.initGrabber(1280, 720);
    
    tracker.setup();
    tracker.setRescale(.5);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()) {
        tracker.update(toCv(cam));
    }
    //get eyes position
    leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
    rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
}

//--------------------------------------------------------------
void ofApp::draw(){
    mouthImage = grabMouth();
    cam.draw(0, 0);
    ofSetLineWidth(2);
    //tracker.draw();
    drawMouth(leftEye, mouthImage);
    drawMouth(rightEye, mouthImage);
    
    //ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void ofApp::drawMouth(ofVec2f eye, ofImage mouth){
    ofEnableAlphaBlending();
    int mWidth = mouth.getWidth() / 2;
    int mHeight = mouth.getHeight() / 2;
    mouth.draw(eye.x - mWidth, eye.y - mHeight);
    ofEnableAlphaBlending();
}

ofImage ofApp::grabMouth(){
    ofPolyline mouthProfile = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
    ofPixels pixels;
    cam.getTexture().readToPixels(pixels);
    ofRectangle mouthBox = mouthProfile.getBoundingBox();
    pixels.crop(mouthBox.x,mouthBox.y,mouthBox.width,mouthBox.height);
    
    ofPixels alphaPixels;
    alphaPixels.allocate(pixels.getWidth(), pixels.getHeight(), OF_IMAGE_COLOR_ALPHA);
    int totalPixels = pixels.getWidth()*pixels.getHeight();
    for (int x = 0; x < pixels.getWidth(); x++){
        for (int y = 0; y < pixels.getHeight(); y++){
            ofPoint checkpoint = ofPoint(x+mouthBox.x,y+mouthBox.y);
            if(mouthProfile.inside(checkpoint)){
                ofColor c = pixels.getColor(x,y);
                alphaPixels.setColor(x,y,c);
            } else {
                ofColor transparent = ofColor(0,0,0,0);
                alphaPixels.setColor(x,y,transparent);
            }
        }
    }
    ofImage videoImage;
    videoImage.setFromPixels(alphaPixels);
    return videoImage;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
