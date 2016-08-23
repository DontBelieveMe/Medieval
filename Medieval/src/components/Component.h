#pragma once

#include <unordered_map>
#include "../introspection/IntrospectionManager.h"

// __COUNTER__ is not actually standard but should be alright for GCC and MSVC
#define COMPONENT(TYPE) \
	static constexpr int static_id = __COUNTER__; \
	TYPE() { REGISTER_TYPE(TYPE); REGISTER_MEMBER(TYPE, id);}

struct Component
{
	virtual void Create(GameObject *object) = 0;
	virtual void Update(GameObject *object) = 0;
	virtual void Destroy() {};

	int id;
};
