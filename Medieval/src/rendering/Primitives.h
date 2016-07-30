#pragma once

#include "../includes.h"
#include "Shader.h"
namespace Primitives
{
	namespace detail
	{
		extern ShaderProgram *primShader;
		extern GLuint vao_cube, vbo_cube, ibo_cube;
		extern GLuint vao_cube_wf, vbo_cube_wf, ibo_cube_wf;
		void tryInit();
	}

	void fillCube(const glm::mat4& view, float x, float y, float z, float w, float h, float depth);
	void drawCube(const glm::mat4& view, float x, float y, float z, float w, float h, float depth);
}