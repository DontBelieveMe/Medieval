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
	const float speed = 0.8f, rot_speed = 0.003f;

	yaw += Input::MousePosDelta().x * rot_speed;
	while (yaw > pi) yaw -= float(2 * pi);
	while (yaw < -pi) yaw += float(2 * pi);

	pitch -= Input::MousePosDelta().y * rot_speed;
	if (pitch < -pi/2) pitch = (float) -pi/2;
	else if (pitch > pi/2) pitch = (float) pi/2;

    glm::vec3 delta;

    delta.y = float(Keys::up.Down() - Keys::down.Down());

	float yaw_sin = std::sin(yaw),
	      yaw_cos = std::cos(yaw);

    delta.x =  yaw_sin * (Keys::forward.Down() - Keys::back.Down()) + yaw_cos * (Keys::right.Down() - Keys::left.Down());
    delta.z = -yaw_cos * (Keys::forward.Down() - Keys::back.Down()) + yaw_sin * (Keys::right.Down() - Keys::left.Down());

    position += delta * speed;
}
glm::mat4 FreeCamera::createView()
{
	glm::mat4 out;
	out = glm::rotate(out, yaw, up);
	out = glm::rotate(out, pitch, {-std::cos(yaw), 0, -std::sin(yaw)});
	out = glm::translate(out, -position);

	return out;
}
