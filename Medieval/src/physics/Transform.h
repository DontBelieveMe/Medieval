#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

struct Transform
{
	glm::vec3   position;
	glm::vec3   scale = glm::vec3(1, 1, 1);
};