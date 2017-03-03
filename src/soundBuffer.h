#include <cstring>
#include <iostream>
#include "ofMain.h"

class soundBuffer
{
public:
	soundBuffer();
	~soundBuffer();

	bool read(string filePath);
	void resetPosition();
	int getNumChannels();
	bool hasNext();
	float next();

private:
	float* buffer;
	int position;

	//A lot of data points from the wave file we read in, which we may or may not care about
	int chunkSize;
	int subChunkSize;
	short format;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
	int dataSize;
};
