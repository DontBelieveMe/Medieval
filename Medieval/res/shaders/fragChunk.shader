#version 330

in vec3 v_color;
in vec3 v_normal;

out vec4 color;

void main()
{
	const vec3 l_src = normalize(vec3(10,7,4));
	float l = dot(l_src, v_normal) / 2. + .5;
	color = vec4(v_color * (.5 + .5 * l), 1);
}