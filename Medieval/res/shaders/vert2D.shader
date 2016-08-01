#version 330

layout(location = 0) in vec3 vertPos;   /*changed attribs to be the same as vert.shader, otherwise AdditionalVAO cannot work, since it is supposed to be used for both 2D, and 3D*/
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal; /*Normals are obviously ignored for 2D rendering*/

out vec2 passUv;

uniform mat4 ortho;
uniform mat4 model;
uniform vec2 tileWH;
uniform ivec4 tile;

void main()
{
	vec2 tileUv = ((texCoord * tile.zw) + tile.xy);
    passUv = vec2(tileUv.x / tileWH.x, tileUv.y / tileWH.y);
	gl_Position = ortho * model * vec4(vertPos, 1.0);
}