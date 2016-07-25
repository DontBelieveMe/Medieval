#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 model;

layout(location = 0) in vec3 pos;

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0f);
}