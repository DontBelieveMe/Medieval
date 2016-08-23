#pragma once

#include <bullet/btBulletDynamicsCommon.h>

#include "../NewComponent.h"
#include "../../physics/PhysicsWorld.h"

struct RigidBodyComponent : Component
{
	COMPONENT(RigidBodyComponent, 1)
	
	virtual void Create(GameObject *object)
	{
		// 2, 6, 1
		collider = new btCylinderShape(btVector3(2, 6, 1));

		glm::vec3& position = object->transform.position;
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
			btVector3(inertia.x, inertia.y, inertia.z)
			);

		rigidbody = new btRigidBody(construct);
		PhysicsWorld::Get()->AddRigidBody(rigidbody);
		
	}

	virtual void Update(GameObject *object)
	{
		btTransform transform;
		rigidbody->getMotionState()->getWorldTransform(transform);
		btVector3 pos = transform.getOrigin();
		object->transform.position.x = pos.x();
		object->transform.position.y = pos.y();
		object->transform.position.z = pos.z();
	}

	virtual void Destroy()
	{
		PhysicsWorld::Get()->RemoveRigidBody(rigidbody);
		delete motion_state;
		delete collider;
		delete rigidbody;
	}


	glm::vec3 inertia;
	float	  mass;

	// The size of the entity, from the top left corner.
	glm::vec3 bounds;

private:
	btDefaultMotionState *motion_state;
	btRigidBody			 *rigidbody;
	btCollisionShape     *collider;

public:
	void AddForce(const glm::vec3& force)
	{
		rigidbody->applyCentralForce(btVector3(force.x, force.y, force.z));
	}

	void AddForce(const glm::vec3& force, const glm::vec3& relative_pos)
	{
		rigidbody->applyForce(btVector3(force.x, force.y, force.z), btVector3(relative_pos.x, relative_pos.y, relative_pos.z));
	}

public:
	static void RegisterMembers()
	{
		REGISTER_MEMBER(RigidBodyComponent, mass);
		REGISTER_MEMBER(RigidBodyComponent, inertia);
		REGISTER_MEMBER(RigidBodyComponent, bounds);
	}
};

FINISH_COMPONENT(RigidBodyComponent);