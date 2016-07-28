/*
* @author Barney Wilks
*/
#pragma once

#include "../includes.h"

#include "Source.h"

class AudioArea
{
private:
	int radius;
	glm::vec2 position;	// this is the centre of the circle.

	std::vector<Source*> sources;
	std::vector<bool>    conditions;
public:
	AudioArea(float x, float y, int radius);

	void addSource(Source *source);
	
	void addCondition(bool condition);		// i dont think this is the best way to do it...
	
	void play();

	void update();
};