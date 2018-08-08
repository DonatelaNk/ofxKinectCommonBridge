#include "ofApp.h"

using namespace glm;

#define WIDTH 640
#define HEIGHT 480


ofApp::ofApp() : pointcloud(kinect) {
};


void ofApp::setup() {
	ofBackground(0);
	ofSetFrameRate(60);
	ofDisableArbTex();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;

	// init kinect
	kinect.initSensor();
	kinect.initColorStream(WIDTH, HEIGHT); 
	//kinect.initIRStream(WIDTH, HEIGHT);	
	kinect.initDepthStream(WIDTH, HEIGHT, false, true);
	kinect.setUseWorldMap(true);
	kinect.start();

	// point cloud 
	pointcloud.setup(WIDTH, HEIGHT);

	cam.setNearClip(0.01);
	cam.setFarClip(10000);
	cam.setFov(20);
	cam.setDistance(1);

	// gui
	gui.setup();
	gui.add(pointcloud.parameters);
}


void ofApp::update() {
	kinect.update();
	pointcloud.update();
}


void ofApp::draw() {
	float scale = 0.5f;
	float w = WIDTH, h = HEIGHT;
	w *= scale; h *= scale;

	// maps
	kinect.draw(0, 0, w, h);
	kinect.drawDepth(w, 0, w, h);
	kinect.drawWorld(w*2, 0, w, h);
	pointcloud.getFboNormals().draw(w*3, 0, w, h);
		
	ofPushStyle();
	ofNoFill();
	for (int j = 0; j < 4; j++) {
		ofDrawRectangle(w*j, 0, w, h);
	}
	ofPopStyle();

	// pointcloud
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	ofEnableDepthTest();
	//glPointSize(2.0);
	cam.begin();
	{
		pointcloud.draw();

		ofDrawAxis(0.5);
	}
	cam.end();
	ofDisableDepthTest();
	glDisable(GL_CULL_FACE);

	// gui
	gui.draw();
}


void ofApp::exit() {
	kinect.stop();
}


void ofApp::keyPressed(int key) {
	
}
