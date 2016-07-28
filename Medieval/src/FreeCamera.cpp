#include "FreeCamera.h"

#include "Input.h"
#include <math.h>

FreeCamera::FreeCamera()
{
	position = glm::vec3(0, 0, 0);
}

FreeCamera::~FreeCamera()
{
}
const float speed = 0.4f;
void FreeCamera::tick()
{
	if (Input::keyDown(GLFW_KEY_W)) {
		position.z -= speed;
	}if (Input::keyDown(GLFW_KEY_S))
		position.z += speed;
	if (Input::keyDown(GLFW_KEY_A))
		position.x -= speed;
	if (Input::keyDown(GLFW_KEY_D))
		position.x += speed;
	if (Input::keyDown(GLFW_KEY_LEFT_CONTROL))
		position.y -= speed;
	if (Input::keyDown(GLFW_KEY_SPACE))
		position.y += speed;

	glm::vec2 mousePos = Input::mousePos();

	/*float horizontal = std::cos(pitch) * speed;
	position.x += cos(yaw) * horizontal;
	position.z += sin(yaw) * horizontal;
	position.y += sin(pitch) * speed;
	position.x -= sin(yaw) * speed;
	position.z += cos(yaw) * speed;
}*/
}
glm::mat4 FreeCamera::createView()
{
	glm::mat4 out = glm::mat4(1.f);
//	out = glm::rotate(out, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
	out = glm::translate(out, -position);
	return out;
}
