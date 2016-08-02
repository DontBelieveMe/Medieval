#include "Primitives.h"

ShaderProgram *Primitives::detail::primitive_shader = NULL;
GLuint Primitives::detail::vao_cube = 0;
GLuint Primitives::detail::vbo_cube = 0;
GLuint Primitives::detail::ibo_cube = 0;
GLuint Primitives::detail::vao_cube_wf = 0;
GLuint Primitives::detail::vbo_cube_wf = 0;
GLuint Primitives::detail::ibo_cube_wf = 0;

void Primitives::FillCube(const glm::mat4& view, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &color)
{
	detail::TryInit();
	detail::primitive_shader->Use();
	detail::primitive_shader->UploadMatrix4f("projection", perspective_matrix);
	detail::primitive_shader->UploadMatrix4f("view", view);
	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	detail::primitive_shader->UploadMatrix4f("model", model);

	detail::primitive_shader->UploadVector3f("u_color", color);

	glBindVertexArray(detail::vao_cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, detail::ibo_cube);
	glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, (void*)0);
}

void Primitives::DrawCube(const glm::mat4& view, const glm::vec3 &pos, const glm::vec3 &scale, const glm::vec3 &color)
{
	detail::TryInit();
	// TODO: Restore GL state to what it was before all the
	// primitve binding stuff has been done.
	// This will mean this function can be calld from anywhere in the code
	// and not cause undefined/strange behaviour with other draw calls.
	// [Note]: Do this with fillCube also.
	detail::primitive_shader->Use();
	detail::primitive_shader->UploadMatrix4f("projection", perspective_matrix);
	detail::primitive_shader->UploadMatrix4f("view", view);

	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	detail::primitive_shader->UploadMatrix4f("model", model);
	detail::primitive_shader->UploadVector3f("u_color", color);

	glBindVertexArray(detail::vao_cube_wf);
	glDrawArrays(GL_LINES, 0, 12*2);
}

void Primitives::detail::TryInit()
{
	if (!primitive_shader) {
		primitive_shader = new ShaderProgram("res/shaders/vertPrimitve.shader", "res/shaders/fragPrimitive.shader");
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
	}
}