#include "PhysicsWorld.h"

#include "../components/ObjectFactory.h"

#include "../components/components/RigidBodyComponent.h"

PhysicsWorld::PhysicsWorld()
{
	broadphase = new btDbvtBroadphase();
	collision_config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_config);

	solver = new btSequentialImpulseConstraintSolver();
	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
	
	// TODO: unhardcode...
	dynamics_world->setGravity(btVector3(0, -9.8f, 0));
}

PhysicsWorld::~PhysicsWorld()
{

}


void PhysicsWorld::Delete()
{
	delete dynamics_world;
	delete broadphase;
	delete collision_config;
	delete dispatcher;
	delete solver;
}

void PhysicsWorld::Tick()
{
	dynamics_world->stepSimulation(1.f / 60.f, 10);
	for (auto& object_pair : ObjectFactory::Get()->GetObjectsMap())
	{
		if (object_pair.second.GetComponentFast<RigidBodyComponent>() != NULL)
		{
			object_pair.second.Update();
		}
	}
}