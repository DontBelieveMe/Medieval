#version 330

layout(location = 0) in vec3 pos;
/*layout(location = 1) in vec3 col; all 2d rendering is texture based, if a multiply-colour is needed, pass it through a uniform*/
layout(location = 1) in vec2 uv;

out vec2 passUv;

uniform mat4 ortho;
uniform mat4 model;
uniform vec2 tileWH;
uniform vec4 tile;

void main()
{
	vec2 tileUv = ((uv * tile.zw) + tile.xy);
    passUv = vec2(tileUv.x / tileWH.x, tileUv.y / tileWH.y);
	gl_Position = ortho * model * vec4(pos, 1.0);
}