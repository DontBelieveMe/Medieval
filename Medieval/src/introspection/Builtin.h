#pragma once

#include "IntrospectionManager.h"
#include <glm/glm.hpp>

inline void RegisterBuiltins()
{
	REGISTER_TYPE(int);
	REGISTER_TYPE(float);
	REGISTER_TYPE(double);
	REGISTER_TYPE(long);
	REGISTER_TYPE(char);
	REGISTER_TYPE(std::string);
	REGISTER_TYPE(glm::vec3);
	REGISTER_TYPE(glm::vec4);
	REGISTER_TYPE(glm::vec2);
}