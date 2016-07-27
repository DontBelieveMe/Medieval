/*
* A Buffer is like a 'CD'. It stores the actual sound
* data that is loaded in from the wav.
*
* ---------
*   Notes
* ---------
* - Currently the buffer will dispose of the wav file *byte* data when
*   It is finished with it. How rude and materialist. Has it even heard of recycling!
*
* @author Barney Wilks 
*/

#pragma once

#include "OpenAL/al.h"
#include "WaveFile.h"

struct Buffer
{
	ALuint id;
	Buffer(wave::WaveFile& file)
	{
		alGenBuffers(1, &id);
		alBufferData(id, file.getFormat(),
			file.data(), file.getSize(), file.getFreq());
		file.destroy();
	}

	void destroy() { alDeleteBuffers(1, &id); }
};