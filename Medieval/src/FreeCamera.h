#pragma once

#include "Utils.h"

class FreeCamera
{
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float pitch = 1;
	float yaw = 1;

public:
	FreeCamera();
	~FreeCamera();

	void tick();

	glm::mat4 createView();
};

