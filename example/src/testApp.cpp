#include "testApp.h"


void testApp::setup() {
	ofBackground(0);
	ofSetFrameRate(60);
	//ofSetVerticalSync(true);
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	for	(int i = 0; i < NUM_DEVICES; i++) {
		kinects[i].initSensor(i);

		// caveats:
		// - color and IR streams can't be inited at the same time. choose one
		// - depth map is unstable if IR stream is not inited (meaybe a driver issue?)
		// - watch out for mapping streams with different sizes
		// - you can only map to color if color stream is inited
		// - color stream is always 640x480 regardless of the value passed on initialization 

		if (USE_IR_STREAM) {
			// clean depth map
			kinects[i].initIRStream(kinectWidth, kinectHeight);
			kinects[i].initDepthStream(kinectWidth, kinectHeight, false, false);
		}
		else {
			// map depth to color (depth map will be noisy)
			kinects[i].initColorStream(kinectWidth, kinectHeight);
			kinects[i].initDepthStream(kinectWidth, kinectHeight, false, true);
		}		

		// calculate real world coordinates?
		kinects[i].setUseWorldMap(true);

		kinects[i].setDepthClipping(500, 4000);

		kinects[i].start();
	}
}


void testApp::update() {
	for (int i = 0; i < NUM_DEVICES; i++) {
		kinects[i].update();
	}
}


void testApp::draw() {
	float w = 320, h = 240;

	for (int i = 0; i < NUM_DEVICES; i++) {
		kinects[i].draw(0, h*i, w, h);
		kinects[i].drawRawDepth(w, h*i, w, h);
		kinects[i].drawDepth(w*2, h*i, w, h);
		kinects[i].drawWorld(w*3, h*i, w, h);
		
		ofPushStyle();
		ofNoFill();
		for (int j = 0; j < 4; j++) {
			ofDrawRectangle(w*j, h*i, w, h);
		}
		ofPopStyle();
	}
}


void testApp::exit() {
	for (int i = 0; i < NUM_DEVICES; i++) {
		kinects[i].stop();
	}

}


void testApp::keyPressed(int key) {
	
}
