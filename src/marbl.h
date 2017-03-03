#include "ofMain.h"
#include "ofxBox2d.h"

#define MARBLE_RADIUS		25

class marbl
{
	private:
		const static ofColor COLORS[];
		const static string TEXTURE_FILES[];
		const static string SOUND_FILES[];

		float angle;
		float amplitude;
		ofImage texture;
		ofSoundPlayer sample;
		ofSpherePrimitive sphere;
		ofxBox2dCircle physics;

	public:
		void draw();

		void playSound();

		float getAngle();
		float getAmplitude();
		float getXPos();
		float getYPos();

		void setPos(float xPos, float yPos);
		void setVelocity(float xVel, float yVel);
		void syncToPhysics();

		marbl(int type, float xPos, float yPos, ofxBox2d* box2d);
		~marbl();
};

//Global utility functions for translating between rectangular and polar coordinates
float posToAngle(float x, float y);
float posToAmplitude(float x, float y);
float polarToX(float angle, float amplitude);
float polarToY(float angle, float amplitude);
