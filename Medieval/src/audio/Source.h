/* 
* This file basically acts as the 'CD player'
* It will play supplied buffers. 
* This can be moved around and can basically control the sound
* For example pitch, volume, looping etc..
*
* ------------
*     Notes
* ------------
* - Things like fade in, fade out etc,
*   are not actually done yet :P
*
* @author Barney Wilks
*/

#pragma once

#include "OpenAL/al.h"
#include "Buffer.h"

#include <glm/vec3.hpp>

class Source
{
private:
	ALuint id;
	bool looping;

public:
	float volume = 1.f;
	float pitch = 1.f;
	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);

public:
	Source();

	void destroy();

	void play(const Buffer& buffer, bool fadeIn);
	void play(Buffer *buffer, bool fadeIn);
	void play();

	void pause();
	void setVolume(float volume);
	void setPitch(float pitch);
	void setPosition(const glm::vec3& position);
	void setVelocity(const glm::vec3& velocity);

	inline bool isLooping() const { return looping; }
	bool isPlaying() const;
	void setLooping(bool loop);
	void stop();
};