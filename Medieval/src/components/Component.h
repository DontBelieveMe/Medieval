#pragma once

#include <unordered_map>
#include "../introspection/IntrospectionManager.h"
#include "../introspection/Serializable.h"

// __COUNTER__ is not actually standard but should be alright for GCC and MSVC
#define COMPONENT(TYPE) \
	static constexpr int static_id = __COUNTER__; \
	SERIALIZABLE(TYPE); \
	TYPE() { REGISTER_TYPE(TYPE); \
	REGISTER_MEMBER(TYPE, id); \
	/*SerializationManager::Get()->MapInstance<TYPE>(#TYPE, &TYPE::CreateObject);*/ }

struct Component : public Serializable
{
	virtual void Create(GameObject *object) = 0;
	virtual void Update(GameObject *object) = 0;
	virtual void Destroy() {};
	virtual ~Component() {}

	int id;
};
