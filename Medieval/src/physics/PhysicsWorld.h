#pragma once

#include <bullet/btBulletDynamicsCommon.h>

class PhysicsWorld
{
private:
	btDiscreteDynamicsWorld *dynamics_world;

public:
	PhysicsWorld();
	~PhysicsWorld();

	void Tick();
	void Delete();

	void RegisterAllObjects();

	// For the time being there is only one world - this may change in the future for 'interiors' etc.
	static PhysicsWorld *Get() { static PhysicsWorld s; return &s; }
};
