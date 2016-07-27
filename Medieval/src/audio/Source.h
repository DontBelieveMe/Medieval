/*
 * @author Barney Wilks
*/

#pragma once

#include "OpenAL/al.h"

class Source
{
private:
	ALuint id;
public:
	Source();

	void destroy();
	

	void play(ALuint buffer);

};