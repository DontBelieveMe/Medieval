#version 330

out vec4 fragColor;

in mat4 Model;
in vec3 Normal;
in vec3 VertPos;

void main()
{
    vec3 tnorm = normalize(mat3(Model) * Normal);
    vec3 eyeCoords = vec3(Model * vec4(VertPos, 1.0));
    vec3 s = normalize(vec3(0.0, 0.0, 0.0) - eyeCoords);
    vec3 diffuse = vec3(0.5, 0.5, 0.5) * max(dot(s, tnorm), 0.0);
    diffuse += vec3(0.25, 0.25, 0.25);

	fragColor = vec4(vec3(0.0, 1.0, 0.0) * diffuse, 1.0);
}