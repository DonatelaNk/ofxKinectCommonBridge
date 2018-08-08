
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofGLWindowSettings settings;
	settings.setGLVersion(4, 1);
	ofCreateWindow(settings);

	ofRunApp(new ofApp());

}
