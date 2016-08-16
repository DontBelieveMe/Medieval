#pragma once

#include "../NewComponent.h"

struct CollidableComponent : Component
{
	virtual void Create() { }
	virtual void Update(GameObject *object);
};

struct RigidBodyComponent : Component
{
	virtual void Create() {}
	virtual void Update(GameObject *object) {
		object->position.x += 1.f;
	}
};
