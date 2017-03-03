#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Need screen height/width
	int windowHeight = ofGetWindowHeight();
	int windowWidth = ofGetWindowWidth();

	camera.enableOrtho();

	//Init graphics
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0,0,0);
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofDisableAlphaBlending();
	ofSetCircleResolution(100); //Fix circle resolution for large circles

	//Init box2d
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds(-windowWidth / 2, -windowHeight / 2, windowWidth, windowHeight);
	box2d.setFPS(60.0);

	//Init other vars
    curFrame = 0;
    activeMarble = NULL;
    isFlickMode = false;
	for(int i = 0; i < NUM_DIVISIONS; i++)
	{
		divisionLastTriggered[i] = 0;
	}

	//Init marbles
	float cutoff = (windowWidth / 2) - CIRCLE_RADIUS - 2 * MARBLE_RADIUS;
	marbles.reserve(NUM_MARBLES);
	for(int i = 0; i < NUM_MARBLES; i++)
	{
		float xPos = 0;
		float yPos = 0;
		do
		{
			xPos = ofRandom(MARBLE_RADIUS * 1.5, cutoff);
			if(ofRandomf() > 0) //50/50 split
				xPos = windowWidth - xPos;
			yPos = ofRandomHeight() * 0.9 + 0.05 * windowHeight; //5% buffer at screen borders
		} while(!isPointClearForMarble(xPos, yPos));
		marbl* newMarble = new marbl((int)ofRandom(0,10), xPos - (windowWidth / 2), yPos - (windowHeight / 2), &box2d);
		marbles.push_back(newMarble);
	}

}

void ofApp::exit()
{

}

//--------------------------------------------------------------
void ofApp::update(){
	//Update physics
	box2d.update();

	//Push physics back to marbles
	for(int i = 0; i < marbles.size(); i++)
	{
		marbles[i]->syncToPhysics();
	}

	//Update circle, play sounds if appropriate
	float timeStamp = ofGetElapsedTimef();
    if(timeStamp - lastFrameChangeTime > BEAT_TIME)
	{
		curFrame++;
		if(curFrame >= NUM_DIVISIONS)
			curFrame = 0;
		lastFrameChangeTime = timeStamp;
		divisionLastTriggered[curFrame] = timeStamp;

		float minAngle = ((float)curFrame / (float)NUM_DIVISIONS) * 2 * PI;
		float maxAngle = ((float)(curFrame + 1) / (float)NUM_DIVISIONS) * 2 * PI;
		for(int i = 0; i < marbles.size(); i++)
		{
			if(marbles[i]->getAmplitude() <= CIRCLE_RADIUS &&
				marbles[i]->getAngle() >= minAngle && marbles[i]->getAngle() < maxAngle)
			{
				marbles[i]->playSound();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	int windowHeight = ofGetWindowHeight();
	int windowWidth = ofGetWindowWidth();

	ofSetupScreenOrtho(windowWidth, windowHeight, -MARBLE_RADIUS * 2, MARBLE_RADIUS * 2);

	ofPushStyle();
		ofPushMatrix();

			//We want to consider the center of the screen to be (0,0)
			ofTranslate(windowWidth / 2, windowHeight / 2);

			//Draw the sequencer circle
			drawCircle();

			//Draw the marbles
			ofPushStyle();
				ofPushMatrix();
					for(int i = 0; i < marbles.size(); i++)
					{
						marbl* curMarble = marbles[i];
						curMarble->draw();
					}
				ofPopMatrix();
			ofPopStyle();

		ofPopMatrix();
	ofPopStyle();

	if(isFlickMode)
		ofDrawBitmapString("Click and drag on a marble to flick it.", 10, 20);
	else
		ofDrawBitmapString("Drag and drop marbles to move them.", 10, 20);
	ofDrawBitmapString("[Spacebar] to change modes.", 10, 35);
}

void ofApp::drawCircle() {
	ofPushStyle();
		ofPushMatrix();
			//Outer circle
			ofSetColor(255, 255, 255);
			ofNoFill();
			ofSetLineWidth(5);
			ofCircle(0, 0, CIRCLE_RADIUS);

			//Circle dividers
			for(int i = 0; i < NUM_DIVISIONS; i++)
			{
				float angle = (float)i * 2.0 * PI / NUM_DIVISIONS;
				float x = polarToX(angle, CIRCLE_RADIUS);
				float y = polarToY(angle, CIRCLE_RADIUS);
				ofLine(0, 0, x, y);
			}

			for(int i = 0; i < NUM_DIVISIONS; i++)
			{
				float shade = 255 - (((ofGetElapsedTimef() - divisionLastTriggered[i]) / (4 * BEAT_TIME)) * 255.0);
				if(shade > 0)
				{
					ofPath path = ofPath();
					path.setCircleResolution(100);
					float angleStart = (float)i * 360.0 / NUM_DIVISIONS;
					float angleEnd = (float)(i + 1) * 360.0 / NUM_DIVISIONS;
					path.arc(0, 0, CIRCLE_RADIUS, CIRCLE_RADIUS, angleStart, angleEnd);
					path.setColor(ofColor(shade, shade, shade));
					path.draw();
				}
			}

		ofPopMatrix();
	ofPopStyle();
}

bool ofApp::isPointClearForMarble(float x, float y)
{
	for(int i = 0; i < marbles.size(); i++)
	{
		marbl* curMarble = marbles[i];
        float marble_x = curMarble->getXPos();
        float marble_y = curMarble->getYPos();
        float dist = sqrt(pow(x - marble_x, 2) + pow(x - marble_y, 2));
        if(dist < 2 * MARBLE_RADIUS)
		{
            return false;
		}
	}
	return true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//Don't allow mode change while a marble is being dragged
	if(activeMarble != NULL)
		return;

	if(key == 32) //Space key
	{
        isFlickMode = !isFlickMode;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(button != 0)
		return;
	if(activeMarble == NULL)
		return;

	//Translate to world coordinates (center is 0,0)
	int windowHeight = ofGetWindowHeight();
	int windowWidth = ofGetWindowWidth();
	float mouse_x = x - (windowWidth / 2);
	float mouse_y = y - (windowHeight / 2);

	if(!isFlickMode && isPointClearForMarble(mouse_x, mouse_y))
	{
		activeMarble->setPos(mouse_x, mouse_y);
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(button != 0)
		return;

	//Translate to world coordinates (center is 0,0)
	int windowHeight = ofGetWindowHeight();
	int windowWidth = ofGetWindowWidth();
	float mouse_x = x - (windowWidth / 2);
	float mouse_y = y - (windowHeight / 2);

	for(int i = 0; i < marbles.size(); i++)
	{
		marbl* curMarble = marbles[i];
		if(!isFlickMode || curMarble->getAmplitude() > CIRCLE_RADIUS) //Disallow flicking marbles in the circle
		{
			float marble_x = curMarble->getXPos();
			float marble_y = curMarble->getYPos();
			float dist = sqrt(pow(mouse_x - marble_x, 2) + pow(mouse_y - marble_y, 2));
			if(dist < MARBLE_RADIUS)
			{
				activeMarble = curMarble;
				break;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(button != 0)
		return;

	//Translate to world coordinates (center is 0,0)
	int windowHeight = ofGetWindowHeight();
	int windowWidth = ofGetWindowWidth();
	float mouse_x = x - (windowWidth / 2);
	float mouse_y = y - (windowHeight / 2);

	if(isFlickMode && activeMarble != NULL)
	{
		activeMarble->setVelocity(activeMarble->getXPos() - mouse_x, activeMarble->getYPos() - mouse_y);
	}

	activeMarble = NULL;
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
