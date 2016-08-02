#version 330

in vec3 v_color;
in vec3 v_normal;

out vec4 color;

void main()
{
	float l = dot(vec3(0,1,0), v_normal) / 2. + .5;
	color = vec4(v_color * (.5 + .5 * l), 1);
}