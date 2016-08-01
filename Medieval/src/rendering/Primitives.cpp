#include "Primitives.h"

ShaderProgram *Primitives::detail::primShader = NULL;
GLuint Primitives::detail::vao_cube = 0;
GLuint Primitives::detail::vbo_cube = 0;
GLuint Primitives::detail::ibo_cube = 0;
GLuint Primitives::detail::vao_cube_wf = 0;
GLuint Primitives::detail::vbo_cube_wf = 0;
GLuint Primitives::detail::ibo_cube_wf = 0;

static const glm::mat4 projection = glm::perspective(45.f, (float)WIDTH / (float)HEIGHT, 0.001f, 200.0f);

void Primitives::fillCube(const glm::mat4& view, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &color)
{
	detail::tryInit();
	detail::primShader->use();
	detail::primShader->uploadMatrix4f("projection", projection);
	detail::primShader->uploadMatrix4f("view", view);
	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	detail::primShader->uploadMatrix4f("model", model);

	detail::primShader->uploadVector3f("u_color", color);

	glBindVertexArray(detail::vao_cube);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, detail::ibo_cube);
	glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	detail::primShader->halt();

}

void Primitives::drawCube(const glm::mat4& view, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &color)
{
	detail::tryInit();
	detail::primShader->use();
	detail::primShader->uploadMatrix4f("projection", projection);
	detail::primShader->uploadMatrix4f("view", view);
	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	detail::primShader->uploadMatrix4f("model", model);

	detail::primShader->uploadVector3f("u_color", color);

	glBindVertexArray(detail::vao_cube_wf);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 12*2);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	detail::primShader->halt();
}

void Primitives::detail::tryInit()
{
	if (!primShader) {
		primShader = new ShaderProgram("res/shaders/vertPrimitve.shader", "res/shaders/fragPrimitive.shader");
		static constexpr GLfloat cube_vertices[] = {
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
		static constexpr GLuint cube_indices[] = {
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

		glGenVertexArrays(1, &vao_cube);
		glBindVertexArray(vao_cube);

		glGenBuffers(1, &ibo_cube);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &vbo_cube);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);



        static constexpr GLfloat cube_wireframe_vertices[]
        {
            -1,-1,-1,-1,-1, 1,
             1,-1,-1, 1,-1, 1,
            -1, 1,-1,-1, 1, 1,
             1, 1,-1, 1, 1, 1,

            -1,-1,-1,-1, 1,-1,
             1,-1,-1, 1, 1,-1,
            -1,-1, 1,-1, 1, 1,
             1,-1, 1, 1, 1, 1,

             -1,-1,-1, 1,-1,-1,
             -1, 1,-1, 1, 1,-1,
             -1,-1, 1, 1,-1, 1,
             -1, 1, 1, 1, 1, 1,
        };


		glGenVertexArrays(1, &vao_cube_wf);
		glBindVertexArray(vao_cube_wf);

		glGenBuffers(1, &vbo_cube_wf);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_wf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_wireframe_vertices), cube_wireframe_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}