#pragma once

#include "Utils.h"

class FreeCamera
{
  public:
	glm::vec3 position;
	float pitch = 0;
	float yaw = 0;
  private:
	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  public:
	FreeCamera();
	~FreeCamera();

	void tick();

	glm::mat4 createView();
};

