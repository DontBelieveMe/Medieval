#include "Renderer2D.h"

void Renderer2D::createVertexArray(float *data, int size)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	count = (size / sizeof(GLfloat)) / 2;
}

void Renderer2D::draw()
{
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES, 0, count);
	
	glBindVertexArray(0);
}

void Renderer2D::destroy()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}