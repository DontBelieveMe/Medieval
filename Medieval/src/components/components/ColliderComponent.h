#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "../NewComponent.h"

struct ColliderComponent : Component
{
	COMPONENT

	virtual void Create() 
	{
		rect_collider = new btBoxShape();
	}
	
	virtual void Update() {}

private:
	btCollisionShape *rect_collider;
};