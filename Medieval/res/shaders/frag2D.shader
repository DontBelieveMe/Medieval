#version 330

out vec4 out_Col;

in vec2 passUv;

uniform sampler2D textureSampler;

void main()
{
	vec4 col = texture(textureSampler, passUv);
    if (col.w != 1.0) discard;
    out_Col = col;
}