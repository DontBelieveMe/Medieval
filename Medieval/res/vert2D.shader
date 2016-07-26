#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 uv;

out vec3 passCol;
out vec2 passUv;

void main()
{
	passUv = uv;
	passCol = col;
	gl_Position = vec4(pos, 1.0);
}