#pragma once

#include "ofMain.h"
#include "marbl.h"
#include "ofxBox2d.h"

#define NUM_DIVISIONS		16
#define NUM_MARBLES 		20
#define CIRCLE_RADIUS		400 //TODO: generalize for multiple screen resolutions
#define SAMPLE_RATE			44100
#define BUFFER_SIZE			1024
#define NUM_BUFFERS			2
#define TEMPO 				240 						//In beats per minute
#define BEAT_TIME			(60.0 / (float)TEMPO) 	//Seconds per beat

class ofApp : public ofBaseApp{

	public:
		vector<marbl*> marbles;
		marbl* activeMarble;
		bool isFlickMode;

		int curFrame;
		float lastFrameChangeTime;
		float divisionLastTriggered[NUM_DIVISIONS];

		ofCamera camera;
		ofxBox2d box2d;

		void setup();
		void update();
		void draw();
		void exit();

		void drawCircle();
		bool isPointClearForMarble(float x, float y);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
