/*
 * @author Barney Wilks
*/
#pragma once

#include "../includes.h"

#include "Shader.h"

class Renderer2D
{
public:
	Renderer2D(ShaderProgram *shader, const glm::mat4 &ortho);
	Renderer2D(ShaderProgram *shader);

	void createVertexArray(float *data, int size);
	void draw();
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	int    count;
	ShaderProgram *shaderRef;
	glm::mat4 defaultOrtho = glm::ortho(0, WIDTH, 0, HEIGHT);
};