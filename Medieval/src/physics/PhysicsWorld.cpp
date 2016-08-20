#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	btBroadphaseInterface *broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration *collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatch = new btCollisionDispatcher(collisionConfig);

	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
	dynamics_world = new btDiscreteDynamicsWorld(dispatch, broadphase, solver, collisionConfig);
	dynamics_world->setGravity(btVector3(0, -9.8, 0));
}

PhysicsWorld::~PhysicsWorld()
{

}


void PhysicsWorld::Delete()
{
	delete dynamics_world;
}

void PhysicsWorld::Tick()
{

}