
#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1280, 768);
	ofCreateWindow(settings);

	ofRunApp(new testApp());

}
