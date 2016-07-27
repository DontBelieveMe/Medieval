#pragma once

#include "OpenAL/al.h"
#include "WaveFile.h"

struct Buffer
{
	ALuint id;
	Buffer(const wave::WaveFile& file)
	{
		alGenBuffers(1, &id);
		alBufferData(id, file.getFormat(),
			file.data(), file.getSize(), file.getFreq());
	}

	void destroy() { alDeleteBuffers(1, &id); }
};