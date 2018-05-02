#include "ofMain.h"
#include "ofApp.h"

#define DISPLAY_MODE OF_WINDOW // Can be OF_WINDOW or OF_FULLSCREEN

int main() {
	ofSetupOpenGL(800, 600, DISPLAY_MODE); // setup the GL context
	ofSetFrameRate(60); 
	ofRunApp(new racingai::carGame());
}
