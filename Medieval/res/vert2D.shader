#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

void main()
{
	gl_Position = vec4(pos, 1.0);
}