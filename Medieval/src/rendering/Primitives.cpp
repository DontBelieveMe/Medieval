#include "Primitives.h"

ShaderProgram *Primitives::detail::primShader = NULL;
GLuint Primitives::detail::vao = -1;
GLuint Primitives::detail::vbo = -1;
GLuint Primitives::detail::ibo = -1;

static const glm::mat4 projection = glm::perspective(45.f, (float)WIDTH / (float)HEIGHT, 0.001f, 200.0f);

void Primitives::fillCube(const glm::mat4& view, float x, float y, float z, float w, float h, float depth)
{
	static const int numVertices = 12 * 3;
	detail::tryInit();
	detail::primShader->use();
	detail::primShader->uploadMatrix4f("projection", projection);
	detail::primShader->uploadMatrix4f("view", view);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(x, y, z));
	model = glm::scale(model, glm::vec3(w, h, depth));
	detail::primShader->uploadMatrix4f("model", model);
	
	glBindVertexArray(detail::vao);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, detail::ibo);
	glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	
	detail::primShader->halt();

}

void Primitives::drawCube(const glm::mat4& view, float x, float y, float z, float w, float h, float depth)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Primitives::fillCube(view, x, y, z, w, h, depth);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Primitives::detail::tryInit()
{
	if (!primShader) {
		primShader = new ShaderProgram("res/shaders/vertPrimitve.shader", "res/shaders/fragPrimitive.shader");
		std::cout << "Starting" << std::endl;
		static const GLfloat vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
		};
		static const GLuint indices[] = {
			0, 1, 2,
			2, 3, 0,
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3,
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}