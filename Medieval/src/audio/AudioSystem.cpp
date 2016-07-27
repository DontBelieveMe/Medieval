#include "AudioSystem.h"

#include <stdio.h>

#include "../includes.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"

extern "C" {
#include "libsndfile/sndfile.h"
}
#include <stdint.h>

AudioSystem::AudioSystem()
{
	ALCdevice *device;
	ALCcontext *context;
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	alcDestroyContext(context);
	alcCloseDevice(device);
}