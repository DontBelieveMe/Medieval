#pragma once

#include "Source.h"
#include "OpenAL/alc.h"
#include "Buffer.h"
#include "WaveFile.h"
class AudioSystem
{
private:
	ALCdevice *device;
	ALCcontext *context;
	Source *source;
	Buffer *buffer;
	wave::WaveFile *wav;

public:
	AudioSystem();
	void destroy();
	void tick();
};