#include "Source.h"

Source::Source() 
{
	alGenSources(1, &this->id);
	alSourcef(id, AL_GAIN, 1.f);
	alSourcef(id, AL_PITCH, 1.f);
	alSource3f(id, AL_POSITION, 0.f, 0.f, 0.f);
	alSource3f(id, AL_POSITION, 0.f, 0.f, 0.f);
}

void Source::destroy()
{
	alDeleteSources(1, &this->id);
}

void Source::play(ALuint buffer)
{
	alSourcei(id, AL_BUFFER, buffer);
	alSourcePlay(id);
}