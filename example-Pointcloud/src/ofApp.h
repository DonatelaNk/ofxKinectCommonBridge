#pragma once

#include "ofMain.h"
#include "ofxKinectCommonBridge.h"
#include "ofxGui.h"
#include "Pointcloud.h"


class ofApp : public ofBaseApp {
	public:
		ofApp();

		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);

		ofxKinectCommonBridge kinect;

		Pointcloud pointcloud;
		
		ofEasyCam cam;

		ofxPanel gui;
};
