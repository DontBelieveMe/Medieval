#version 330

out vec4 out_Col;

in vec3 passCol;
in vec2 passUv;

uniform sampler2D textureSampler;

void main()
{
	out_Col = texture(textureSampler, passUv);
}