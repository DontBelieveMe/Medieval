#include "AudioSystem.h"

#include <stdio.h>

#include "../includes.h"
#include "OpenAL/al.h"
#include "../Input.h"

const float f = 40;
float xPos = -f;

AudioSystem::AudioSystem()
{
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	wav = new wave::WaveFile("res/audio/NoTimeToShine.wav");
	buffer = new Buffer(*wav);

	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	source = new Source();
	source->setVolume(.5f);
	source->setLooping(true);
//	source->play(buffer, false);
//	source->pause();
	source->setPosition(glm::vec3(0, 0, 0));
}

void AudioSystem::destroy()
{
	source->stop();
	buffer->destroy();
	delete buffer;

	source->destroy();
	delete source;

	delete wav;

	alcDestroyContext(context);
	alcCloseDevice(device);
}
bool right = false;
void AudioSystem::tick()
{
	if (Keys::toggle_audio.Pressed())
	{
		if (source->isPlaying())
			source->pause();
		else
			source->play();
	}
	if (xPos < -f)
		right = false;

	if (xPos > f)
		right = true;

	if (source->isPlaying())
	{
		if (right)
			xPos -= 0.3f;
		else
			xPos += 0.3f;
//		source->setPosition(glm::vec3(xPos, 0, 2));
	}
}