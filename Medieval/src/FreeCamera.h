#pragma once

#include "Utils.h"

class FreeCamera
{
private:
	glm::vec3 position;
	float pitch = 1;
	float yaw = 0;

public:
	FreeCamera();
	~FreeCamera();

	void tick();

	glm::mat4 createView();
};

