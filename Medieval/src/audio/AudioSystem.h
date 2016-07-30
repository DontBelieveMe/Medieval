/*
*--------
*  Note
*--------
* - This file is just a temporary test bed for audio
*   shananagins. It is not a permanant feature and
*   was just to let me (Barney) fiddle with my sound system
*/
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

namespace AudioUtils
{
	inline bool pointInCircle(const glm::vec2& point, const glm::vec2& center, int radius)
	{
		return glm::dot(point - center, point - center) <= radius*radius;
	}
}