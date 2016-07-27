This is where the physics stuff will go, for now we just need simple AABB collision.

We need a PhysicsManager, you could call it that. Or something more like Bullet, and call
it DynamicsWorld, or something.

You can then add a AABBRigidBody, which contains a position, and a size. It's position should be
handled by the PhysicsManager, in the future,  we may add rotationPhysics. Possibly also different
physics shapes such as CylinderRigidBody, and SphereRigidBody.

