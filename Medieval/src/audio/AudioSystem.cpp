#include "AudioSystem.h"

#include <stdio.h>

#include "../includes.h"

#include "WaveFile.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"


AudioSystem::AudioSystem()
{
	ALCdevice *device;
	ALCcontext *context;
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	wave::WaveFile file("res/audio/bounce.wav");
	ALuint buffer;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, file.getFormat(), file.data(), file.getSize(), file.getFreq());
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	ALuint source;
	alGenSources(1, &source);
	alSourcef(source, AL_GAIN, 1.f);
	alSourcef(source, AL_PITCH, 1.f);
	alSource3f(source, AL_POSITION, 0, 0, 0);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	/*alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);*/
}