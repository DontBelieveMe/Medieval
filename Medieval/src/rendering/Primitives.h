#pragma once
#include <../extern/glad/glad.h>
#include <glm\glm.hpp>

//Someone needs to write up the cube data stuff - it's not that difficult, I'm just a bit lazy :P
//It WILL use GL_TRIANGLE_FAN so don't even think about trying to use the element buffer
//

struct Primitive
{
public:
	Primitive(GLuint VBO, GLuint VAO, unsigned int numIndicies)
		: _VBO(VBO), _VAO(VAO), numIndicies(numIndicies) {}
	
	void Render() { glBindVertexArray(_VAO); glDrawArrays(GL_TRIANGLE_FAN, 0, numIndicies); }
protected:
	GLuint _VBO, _VAO;
	unsigned int numIndicies;
};

class Primitives
{
public:
	static void Init(); //Generates the Primitives

	static Primitive &GetQuad() { return *quad; }
	static Primitive &GetUIQuad() { return *uiQuad; }
	//static Primitive &GetCube() { return *cube; }	

private:
	static Primitive *quad;
	static Primitive *uiQuad;
	//static Primitive *cube;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;

		Vertex(glm::vec3 position, glm::vec2 uv)
			: position(position), uv(uv) {}
	};
};