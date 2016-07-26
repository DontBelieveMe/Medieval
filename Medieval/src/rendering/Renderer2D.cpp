#include "Renderer2D.h"

Renderer2D::Renderer2D(ShaderProgram *shader, const glm::mat4 &ortho)
	: shaderRef(shader)
{
	shader->use();
	shader->uploadMatrix4f("ortho", ortho);		// Probably shouldn't hardcode this name/location: TODO change
	shader->halt();
}

Renderer2D::Renderer2D(ShaderProgram *shader)
	: shaderRef(shader)
{
	shader->use();
	shader->uploadMatrix4f("ortho", defaultOrtho);
	shader->halt();
}

void Renderer2D::createVertexArray(float *data, int size)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);
	count = (size / sizeof(GLfloat)) / 8;
	if (count != 3)
		std::cout << "Warning: Renderer2D count is not 3. This could be ok. This is just a helper if things start to not work" << std::endl;
}

void Renderer2D::draw()
{
	shaderRef->use();
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, count);

	glBindVertexArray(0);
	shaderRef->halt();
}

void Renderer2D::destroy()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}