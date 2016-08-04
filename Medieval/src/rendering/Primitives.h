#pragma once
#include <../extern/glad/glad.h>
#include <glm\glm.hpp>

struct Primitive
{
public:
	Primitive(GLuint VBO, GLuint VAO, unsigned int numIndicies, GLenum drawType)
		: _VBO(VBO), _VAO(VAO), numIndicies(numIndicies), drawType(drawType) {}
	
	void Render() { glBindVertexArray(_VAO); glDrawArrays(drawType, 0, numIndicies); }
protected:
	GLuint _VBO, _VAO;
	unsigned int numIndicies;
	GLenum drawType;
};

class Primitives
{
public:
	static void Init(); //Generates the Primitives

	static Primitive &GetQuad() { return *quad; }
	static Primitive &GetUIQuad() { return *uiQuad; }
	static Primitive &GetCube() { return *cube; }	

private:
	static Primitive *quad;
	static Primitive *uiQuad;
	static Primitive *cube;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;

		Vertex(glm::vec3 position, glm::vec2 uv)
			: position(position), uv(uv) {}
	};
};