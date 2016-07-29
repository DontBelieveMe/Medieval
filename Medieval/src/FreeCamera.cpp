#include "FreeCamera.h"

#include "Input.h"
#include <math.h>

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
	const float speed = 0.4f;
	if (Input::keyDown(GLFW_KEY_W)) {
		position += speed * front;
	}if (Input::keyDown(GLFW_KEY_S))
		position -= speed * front;

	if (Input::keyDown(GLFW_KEY_LEFT_CONTROL))
		position.y -= speed;
	if (Input::keyDown(GLFW_KEY_SPACE))
		position.y += speed;

	if (Input::keyDown(GLFW_KEY_A))
		yaw -= speed*0.1;
	if (Input::keyDown(GLFW_KEY_D))
		yaw += speed*0.1;
}
glm::mat4 FreeCamera::createView()
{
	glm::mat4 out;
	out = glm::translate(out, -position);
	out = glm::rotate(out, yaw, up);
	
	return out;
}
