#include "Builtin.h"

void RegisterBuiltins()
{
	REGISTER_TYPE(int);
	REGISTER_TYPE(float);
	REGISTER_TYPE(double);
	REGISTER_TYPE(long);
	REGISTER_TYPE(char);
	REGISTER_TYPE(unsigned int);
	REGISTER_TYPE(unsigned char);
	REGISTER_TYPE(unsigned long);
	REGISTER_TYPE(long long);
	REGISTER_TYPE(wchar_t);
	REGISTER_TYPE(bool);
	REGISTER_TYPE(short);

	REGISTER_TYPE(std::string);
	REGISTER_MEMBER(std::string, length);
	REGISTER_TYPE(glm::vec3);
	REGISTER_MEMBER(glm::vec3, x);
	REGISTER_MEMBER(glm::vec3, y);
	REGISTER_MEMBER(glm::vec3, z);

	REGISTER_TYPE(glm::vec4);
	REGISTER_MEMBER(glm::vec4, x);
	REGISTER_MEMBER(glm::vec4, y);
	REGISTER_MEMBER(glm::vec4, z);
	REGISTER_MEMBER(glm::vec4, w);

	REGISTER_TYPE(glm::vec2);
	REGISTER_MEMBER(glm::vec2, x);
	REGISTER_MEMBER(glm::vec2, y);

	REGISTER_TYPE(glm::mat4);

	REGISTER_TYPE(btVector3);
	REGISTER_TYPE(btQuaternion);
	REGISTER_TYPE(btRigidBody);
	REGISTER_TYPE(btMotionState);
	REGISTER_TYPE(btCollisionShape);
}