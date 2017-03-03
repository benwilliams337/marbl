#include "marbl.h"

const string marbl::TEXTURE_FILES[] = {
    "textures\\texture_0.bmp",
    "textures\\texture_1.bmp",
    "textures\\texture_2.bmp",
    "textures\\texture_3.bmp",
    "textures\\texture_4.bmp",
    "textures\\texture_5.bmp",
    "textures\\texture_6.bmp",
    "textures\\texture_7.bmp",
    "textures\\texture_8.bmp",
    "textures\\texture_9.bmp"
};

const string marbl::SOUND_FILES[] = {
	"wav\\11077__angstrom__c1.wav",
	"wav\\11081__angstrom__d1.wav",
	"wav\\11083__angstrom__e1.wav",
	"wav\\11086__angstrom__f1.wav",
	"wav\\11088__angstrom__g1.wav",
	"wav\\11073__angstrom__a2.wav",
	"wav\\11074__angstrom__b2.wav",
	"wav\\11078__angstrom__c2.wav",
	"wav\\11082__angstrom__d2.wav",
	"wav\\11084__angstrom__e2.wav",
};

marbl::marbl(int type, float xPos, float yPos, ofxBox2d* box2d)
{
	//Init angle and amplitude based on input
	angle = posToAngle(xPos, yPos);
	amplitude = posToAmplitude(xPos, yPos);

	//Init sphere to set radius and random rotation
	sphere.setRadius(MARBLE_RADIUS);
	sphere.rotate(ofRandomuf() * 360.0, 0.0, 1.0, 0.0);
	sphere.rotate(ofRandomuf() * 360.0, 1.0, 0.0, 0.0);

	//Load texture and sound clip
	texture.loadImage(TEXTURE_FILES[type]);
	sample.loadSound(SOUND_FILES[type]);

	//Init Box2D physics model
	physics.setPhysics(3.0, 0.5, 1.0); //density, bounce, friction
	physics.setup(box2d->getWorld(), xPos, yPos, MARBLE_RADIUS);
}

marbl::~marbl()
{
}

void marbl::draw()
{
	ofPushStyle();
		ofPushMatrix();
			//Move to marble's coords
			float x = polarToX(angle, amplitude);
			float y = polarToY(angle, amplitude);
			ofTranslate(x, y, MARBLE_RADIUS);

			sphere.mapTexCoordsFromTexture(texture.getTextureReference());
			texture.getTextureReference().bind();
			sphere.draw();
			texture.unbind();

		ofPopMatrix();
	ofPopStyle();
}

void marbl::playSound()
{
    sample.play();
}

float marbl::getAngle()
{
	return angle;
}

float marbl::getAmplitude()
{
	return amplitude;
}

float marbl::getXPos()
{
	return polarToX(angle, amplitude);
}

float marbl::getYPos()
{
	return polarToY(angle, amplitude);
}

void marbl::setPos(float xPos, float yPos)
{
	angle = posToAngle(xPos, yPos);
	amplitude = posToAmplitude(xPos, yPos);
	physics.setPosition(xPos, yPos);
}

void marbl::setVelocity(float xVel, float yVel)
{
	physics.setVelocity(xVel, yVel);
}

void marbl::syncToPhysics()
{
	//Pull position from physics object and assign to sphere
	ofVec2f physicsPos = physics.getPosition();
	angle = posToAngle(physicsPos.x, physicsPos.y);
	amplitude = posToAmplitude(physicsPos.x, physicsPos.y);

	//Rotate sphere based on velocity
	ofVec2f vel = physics.getVelocity();
	if(vel.x != 0 || vel.y != 0)
	{
		float angle_x = ((vel.x / 60.0f) / (float)MARBLE_RADIUS) * 360.0f / 2.0f * PI;
		float angle_y = -((vel.y / 60.0f) / (float)MARBLE_RADIUS) * 360.0f / 2.0f * PI;
        sphere.rotate(angle_x, 0.0, 1.0, 0.0);
        sphere.rotate(angle_y, 1.0, 0.0, 0.0);
	}

	//Set damping (artificial friction, needs to be set per frame)
	physics.setDamping(0.95f);
}


//Not methods of the marbl class...
float posToAngle(float x, float y)
{
	float result = atan2(y, x);
	//atan2 returns -pi to pi, but we want 0 to 2*pi
	if(result < 0)
		result += 2 * PI;
	return result;
}

float posToAmplitude(float x, float y)
{
	return sqrt(pow(x,2) + pow(y,2));
}

float polarToX(float angle, float amplitude)
{
	return amplitude * cos(angle);
}

float polarToY(float angle, float amplitude)
{
	return amplitude * sin(angle);
}
