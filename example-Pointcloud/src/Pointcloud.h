#pragma once

#include "ofMain.h"
#include "ofxKinectCommonBridge.h"


class Pointcloud {

	public:
		ofParameterGroup parameters;
		ofParameter<float> clippingNear;
		ofParameter<float> clippingFar;
		ofParameter<float> jumpMax;
		ofParameter<int> jumpStep;

		Pointcloud(ofxKinectCommonBridge& kinect);

		void setup(int width, int height, int steps = 1);
		void update();
		void draw();
		
		const ofFbo & getFboNormals() { 
			return fboNormals; 
		}

	private:
		int width, height;
		int steps;

		ofxKinectCommonBridge & kinect;

		ofVboMesh mesh;
		ofVboMesh quad;

		ofFbo fboNormals;

		ofShader shaderNormals;
		ofShader shaderPointcloud;

		void createMesh();
		void createFullScreenQuad();
};