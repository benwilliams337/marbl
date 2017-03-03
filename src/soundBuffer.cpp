#include "soundBuffer.h"

soundBuffer::soundBuffer()
{
	position = 0;
}

soundBuffer::~soundBuffer()
{
	delete buffer;
}

void soundBuffer::resetPosition()
{
	position = 0;
}

int soundBuffer::getNumChannels()
{
	return numChannels;
}

bool soundBuffer::hasNext()
{
    return (position < dataSize);
}

float soundBuffer::next()
{
	float result = buffer[position];
	position++;
	return result;
}

//Reads data into the sound buffer from a wave file
//Adapted from the example code posted at http://forum.openframeworks.cc/t/example-of-playing-back-a-sample-in-ofsoundstream/3502/21
//Many thanks to grimus for providing this sample code!
bool soundBuffer::read(string filePath)
{
	string myPath = ofToDataPath(filePath,true).c_str();
    ifstream inFile( myPath.c_str(), ios::in | ios::binary);

    if(!inFile.is_open())
    {
        ofLog(OF_LOG_ERROR,"Error opening file. File not loaded.");
        return false;
    }

    char id[4];
    inFile.read((char*) &id, 4);
    if(strncmp(id,"RIFF",4) != 0)
    {
        ofLog(OF_LOG_ERROR,"Error reading sample file. File is not a RIFF (.wav) file");
        return false;
    }

    inFile.seekg(4, ios::beg);
    inFile.read( (char*) &chunkSize, 4 ); // read the ChunkSize

    inFile.seekg(16, ios::beg);
    inFile.read( (char*) &subChunkSize, 4 ); // read the SubChunk1Size

    inFile.read( (char*) &format, sizeof(short) ); // read the file format.  This should be 1 for PCM
    if(format != 1) {
        ofLog(OF_LOG_ERROR, "File format should be PCM, sample file failed to load.");
        return false;
    }

    inFile.read( (char*) &numChannels, sizeof(short) ); // read the # of channels (1 or 2)
    inFile.read( (char*) &sampleRate, sizeof(int) ); // read the Samplerate
    inFile.read( (char*) &byteRate, sizeof(int) ); // read the byterate
    inFile.read( (char*) &blockAlign, sizeof(short) ); // read the blockalign
    inFile.read( (char*) &bitsPerSample, sizeof(short) ); // read the bitsperSample

    inFile.seekg(40, ios::beg);
    inFile.read( (char*) &dataSize, sizeof(int) ); // read the size of the data

    // read the data chunk
    char* tempData = new char[dataSize];
    inFile.seekg(44, ios::beg);
    inFile.read(tempData, dataSize);

	//Scale to -1.0 to 1.0
	buffer = new float[dataSize];
    for(int i = 0; i < dataSize; i++)
	{
		buffer[i] = ((float)tempData[i])/128.0;
	}

    inFile.close(); // close the input file
    delete tempData;
    position = dataSize; //Default to not playing

	//cout << myPath << endl;
    cout << bitsPerSample << endl;

    return true;
}
