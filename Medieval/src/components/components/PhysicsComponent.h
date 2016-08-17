#pragma once

#include "../NewComponent.h"

struct CollidableComponent : Component
{
	COMPONENT
	
	virtual void Create() { }
	virtual void Update(GameObject *object);
};

struct RigidBodyComponent : Component
{
	COMPONENT
	
	int test = 100;
	virtual void Create() {}
	virtual void Update(GameObject *object) {
		object->position.x += 1.f;
	}
};
