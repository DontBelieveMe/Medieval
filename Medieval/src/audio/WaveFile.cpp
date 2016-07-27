#include "WaveFile.h"
#include "../Utils.h"

using namespace wave;

bool cmpStrArray(char *one, char *two, int size)
{
	for (int i = 0; i <= size; ++i) {
		if (one[i] != two[i])
			return false;
	}
	return true;
}

WaveFile::WaveFile(const std::string& path)
{
	using namespace wave;
	using namespace wave::detail;
	
	FILE *file = fopen(path.c_str(), "rb");

	if (!file) {
		Error("Error opening wav file! ", path);
	}

	RIFF riff;
	fread(&riff, sizeof(RIFF), 1, file);

	if (!cmpStrArray(riff.chunkId, "RIFF", 3) || !cmpStrArray(riff.format, "WAVE", 3))
	{
		Error("Invalid RIFF or Wave header!");
	}

	FMT format;
	fread(&format, sizeof(FMT), 1, file);
	if (!cmpStrArray(format.subChunk1ID, "fmt ", 3))
	{
		Error("Invalid Wave format!");
	}

	if (format.subChunk1Size > 16)
	{
		fseek(file, sizeof(int16_t), SEEK_CUR);
	}

	DATA dataHeader;
	fread(&dataHeader, sizeof(DATA), 1, file);

	if (!cmpStrArray(dataHeader.subChunk2ID, "data", 3))
	{
		Error("Invalid data header!");
	}

	this->soundData = new byte[dataHeader.subChunk2Size];
	if (!fread(this->soundData, dataHeader.subChunk2Size, 1, file))
	{
		Error("Error loading WAVE audio data!");
	}

	this->size = dataHeader.subChunk2Size;
	this->frequency = format.sampleRate;
	
	if (format.numChannels == 1)
	{
		if (format.bitsPerSample == 8)
		{
			this->format = AL_FORMAT_MONO8;
		} 
		else if(format.bitsPerSample == 16)
		{
			this->format = AL_FORMAT_MONO16;
		}
	} 
	else if (format.numChannels == 2)
	{
		if (format.bitsPerSample == 8)
		{
			this->format = AL_FORMAT_STEREO8;
		} 
		else if (format.bitsPerSample == 16)
		{
			this->format = AL_FORMAT_STEREO16;
		}
	}

	_data = dataHeader;
	_riff = riff;
	_format = format;

	fclose(file);
}

void WaveFile::destroy()
{
	delete[] soundData;
	soundData = nullptr;
}

WaveFile::~WaveFile()
{
	destroy();
}