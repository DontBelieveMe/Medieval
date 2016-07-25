#version 330
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 model;

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out mat4 Model;
out vec3 Normal;
out vec3 VertPos;

void main()
{
    Model = model;
    Normal = normal;
    VertPos = vertPos;

	gl_Position = projection * model * vec4(vertPos, 1.0f);
}