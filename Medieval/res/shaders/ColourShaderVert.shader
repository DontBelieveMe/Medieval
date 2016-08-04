#version 330 core
layout (location = 0) in vec3 position;

out vec4 vertColour;

uniform mat4 MVP;
uniform vec4 Colour;

void main()
{
	gl_Position = MVP * vec4(position, 1.0f);
	vertColour = Colour;
}