#pragma once
#include "ofxKinectCommonBridge.h"
#include "ofMain.h"

#define NUM_DEVICES 2
#define USE_640x480 false
#define USE_IR_STREAM false


class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);

		ofxKinectCommonBridge kinects[NUM_DEVICES];

		const int kinectWidth = USE_640x480 ? 640 : 320;
		const int kinectHeight = USE_640x480 ? 480 : 240;

};
