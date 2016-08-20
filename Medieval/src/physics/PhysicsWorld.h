#pragma once

#include <bullet/btBulletDynamicsCommon.h>

class PhysicsWorld
{
private:
	btDiscreteDynamicsWorld			    *dynamics_world;
	btBroadphaseInterface			    *broadphase;
	btDefaultCollisionConfiguration		*collision_config;
	btSequentialImpulseConstraintSolver *solver;
	btCollisionDispatcher			    *dispatcher;

public:
	PhysicsWorld();
	~PhysicsWorld();

	void Tick();
	void Delete();

	void AddRigidBody(btRigidBody *rigidbody) { dynamics_world->addRigidBody(rigidbody); }
	void RemoveRigidBody(btRigidBody *rigidBody) { dynamics_world->removeRigidBody(rigidBody); }

	// For the time being there is only one world - this may change in the future for 'interiors' etc.
	static PhysicsWorld *Get() { static PhysicsWorld s; return &s; }
};
