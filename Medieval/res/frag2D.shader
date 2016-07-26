#version 330

out vec4 out_Col;
in vec3 passCol;
void main()
{
	out_Col = vec4(passCol, 1.0);
}