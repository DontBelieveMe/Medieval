#include <bullet/btBulletDynamicsCommon.h>
#include <iostream>

inline std::ostream &operator<<(std::ostream &s, const btVector3& obj) { s << "vec3: [" << obj.x() << ',' << obj.y() << ','<< obj.z() << ']'; return s; }

template <typename T>
struct RigidBody;

struct PhysicsWorld
{
	btDiscreteDynamicsWorld *world;
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *collision_config;
	btSequentialImpulseConstraintSolver *solver;
	btCollisionDispatcher	*dispatcher;

	float gravity;

	PhysicsWorld()
		: gravity(-9.8f)
	{
		broadphase = new btDbvtBroadphase();
		collision_config = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collision_config);
		solver = new btSequentialImpulseConstraintSolver();

		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
		world->setGravity(btVector3(0, gravity, 0));
	}

	static PhysicsWorld *GetWorld() { static PhysicsWorld world; return &world; }
};

template <typename T>
struct RigidBody
{
	float x, y, z;
	float mass;

	btDefaultMotionState *motion;
	btRigidBody			 *rigid_body;
	btCollisionShape	 *collider;

	template <typename... Args>
	RigidBody(float x, float y, float z, float mass, const btVector3& inertia, const Args&... args) : x(x), y(y), z(z), mass(mass) 
	{
		collider = new T(args...);
		CreateRigidBody(inertia);
	}

	btRigidBody *CreateRigidBody(const btVector3& inertia)
	{
		motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z)));
		btRigidBody::btRigidBodyConstructionInfo construct(mass, motion, collider, inertia);
		rigid_body = new btRigidBody(construct);
		PhysicsWorld::GetWorld()->world->addRigidBody(rigid_body);
		return rigid_body;
	}

	void Destroy()
	{
		delete motion;
		delete collider;
		delete rigid_body;
	}

	void Tick()
	{
		btVector3 position = rigid_body->getWorldTransform().getOrigin();
		x = position.x();
		y = position.y();
		z = position.z();
	}
};

int main()
{
	RigidBody<btStaticPlaneShape> ground(0, -1, 0, btScalar(0), btVector3(0, 0, 0), btVector3(0, 1, 0), 1.f);
	RigidBody<btSphereShape> sphere(0, 50, 0, 1, btVector3(0.4f, 0.4f, 0.4f), 1.f);
	
	for (int i = 0; i < 300; i++) {
		PhysicsWorld::GetWorld()->world->stepSimulation(1 / 60.0f, 10);
		sphere.Tick();
		std::cout << sphere.y << std::endl;
	}

	system("pause");
	return 0;
}
