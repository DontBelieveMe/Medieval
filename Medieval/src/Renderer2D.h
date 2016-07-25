/*
 * @author Barney Wilks
*/
#pragma once

#include "includes.h"

class Renderer2D
{
public:
	void createVertexArray(float *data, int size);
	void draw();

private:
	GLuint vao;
	GLuint vbo;
	int    count;

};