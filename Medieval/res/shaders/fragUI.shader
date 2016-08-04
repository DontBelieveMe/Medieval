#version 330 core
in vec4 vertColour;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D uiTexture;

void main()
{
	color = texture(uiTexture, TexCoord);
	//color = vertColour;
}