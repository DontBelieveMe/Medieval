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

	static std::pair<GLfloat*, int> triangle(float x, float y, float w, float h)
	{
		GLfloat data[] = {
			x, y, 0.f,	0.f, 0.f, 0.f, 0.f, 1.f,
			x + w, y, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,
			x + (w / 2), y + h, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f
		};
		
		return std::make_pair(data, sizeof(data));
	}

private:
	GLuint vao;
	GLuint vbo;
	int    count;
	ShaderProgram *shaderRef;
};