#pragma once

#include "../NewComponent.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "../../physics/PhysicsWorld.h"

struct RigidBodyComponent : Component
{
	COMPONENT

	virtual void Create(GameObject *object)
	{
		glm::vec3& position = object->position;
		motion_state = new btDefaultMotionState(
			btTransform(
				btQuaternion(0, 0, 0, 1),
				btVector3(position.x, position.y, position.z)
				)
			);

		btRigidBody::btRigidBodyConstructionInfo construct(
			mass,
			motion_state,
			collider,
			inertia
			);

		rigid_body = new btRigidBody(construct);

		PhysicsWorld::Get()->AddRigidBody(rigid_body);
	}

	virtual void Update(GameObject *object)
	{
		btVector3 position = rigid_body->getWorldTransform().getOrigin();
		object->position.x = position.x();
		object->position.y = position.y();
		object->position.z = position.z();
	}

	virtual void Destroy()
	{
		PhysicsWorld::Get()->RemoveRigidBody(rigid_body);
		delete motion_state;
		delete rigid_body;
		delete collider;
	}

	btDefaultMotionState *motion_state;
	btRigidBody			 *rigid_body;
	btCollisionShape     *collider;

	btVector3 inertia;
	float	  mass;
};