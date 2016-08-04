#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;
//out vec4 vertColour;

uniform mat4 translationScale;

void main()
{
	gl_Position = translationScale * vec4(position, 1.0f);
	//vertColour = vec4(0.5f, 0.0f, 0.0f, 1.0f);
	TexCoord = texCoord;
}