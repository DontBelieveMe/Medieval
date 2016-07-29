#include "Source.h"

Source::Source() 
{
	alGenSources(1, &this->id);
	alSourcef(id, AL_GAIN, volume);
	alSourcef(id, AL_PITCH, pitch);
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
}

void Source::destroy()
{
	alDeleteSources(1, &this->id);
}

void Source::play(const Buffer& buffer, bool fadeIn)
{
	setVolume(0.f);
	alSourcei(id, AL_BUFFER, buffer.id);
	alSourcePlay(id);
}

void Source::play(Buffer *buffer, bool fadeIn)
{
	play(*buffer, fadeIn);
}

void Source::setPitch(float pitch)
{
	this->pitch = pitch;
	alSourcef(id, AL_PITCH, pitch);
}

void Source::setPosition(const glm::vec3& position)
{
	pos = position;
	alSource3f(id, AL_POSITION, position.x, position.y, position.z);
}

void Source::setVolume(float volume)
{
	this->volume = volume;
	alSourcef(id, AL_GAIN, volume);
}

void Source::setVelocity(const glm::vec3& velocity)
{
	alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void Source::setLooping(bool loop)
{
	looping = loop;
	ALenum state = loop ? AL_TRUE : AL_FALSE;
	alSourcei(id, AL_LOOPING, state);
}

bool Source::isPlaying() const 
{
	ALenum state;
	alGetSourcei(id, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;    
}

void Source::pause() 
{
	if (isPlaying())
	{
		alSourcePause(id);
	}
}

void Source::play()
{
	if (!isPlaying())
	{
		alSourcePlay(id);
	}
}

void Source::stop()
{
	alSourceStop(id);
}