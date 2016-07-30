#include "FreeCamera.h"

#include "Input.h"
#include <cmath>

FreeCamera::FreeCamera()
{
	position = glm::vec3(0, 0, 0);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
}

FreeCamera::~FreeCamera()
{
}
void FreeCamera::tick()
{
	const float speed = 0.8, rot_speed = 0.03;

	yaw += (Keys::right.down() - Keys::left.down()) * rot_speed;

    glm::vec3 delta;

    delta.y = Keys::up.down() - Keys::down.down();
    delta.y = Keys::up.down() - Keys::down.down();

	float yaw_sin = std::sin(yaw),
	      yaw_cos = std::cos(yaw);

    delta.x =  yaw_sin * (Keys::forward.down() - Keys::back.down());
    delta.z = -yaw_cos * (Keys::forward.down() - Keys::back.down());

    position += delta * speed;
}
glm::mat4 FreeCamera::createView()
{
	glm::mat4 out;
	out = glm::rotate(out, yaw, up);
	out = glm::translate(out, -position);

	return out;
}
