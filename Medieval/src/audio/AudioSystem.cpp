#include "AudioSystem.h"

#include <stdio.h>

#include "../includes.h"
#include "OpenAL/al.h"
#include "../Input.h"

float xPos = 20;
AudioSystem::AudioSystem()
{
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	wav = new wave::WaveFile("res/audio/bounce.wav");
	buffer = new Buffer(*wav);
	
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);

	source = new Source();
	source->setLooping(true);
	source->play(buffer);

	source->setPosition(glm::vec3(xPos, 0, 4));
}

void AudioSystem::destroy()
{
	buffer->destroy();
	delete buffer;
	
	source->destroy();
	delete source;

	wav->destroy();
	delete wav;

	alcDestroyContext(context);
	alcCloseDevice(device);
}
bool right = true;
void AudioSystem::tick()
{
	if (Input::keyPressed(GLFW_KEY_P))
	{
		if (source->isPlaying())
			source->pause();
		else
			source->play();
	}
	if (xPos < -20)
		right = false;
	
	if (xPos > 20)
		right = true;
		
	if (source->isPlaying())
	{
		if (right)
			xPos -= 0.3f;
		else
			xPos += 0.3f;
		std::cout << xPos << std::endl;
		source->setPosition(glm::vec3(xPos, 0, 4));
	}
}