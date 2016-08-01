#version 330

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;

out vec3 v_color;
out vec3 v_normal;

void main()
{
	v_normal = a_normal;
	v_color = a_color;
	gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
}