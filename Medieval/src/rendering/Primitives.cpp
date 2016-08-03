#include "Primitives.h"
#include <vector>

Primitive *Primitives::quad;
Primitive *Primitives::uiQuad;
//Primitive *Primitives::cube;

void Primitives::Init()
{
	//Basic Variables
	GLuint VBO, VAO;
	std::vector<Vertex> verts;	

	//Quad	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	
	verts.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0), glm::vec2(0, 0)));
	verts.push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0), glm::vec2(0, 1)));
	verts.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0), glm::vec2(1, 1)));
	verts.push_back(Vertex(glm::vec3( 0.5f, -0.5f, 0), glm::vec2(1, 0)));	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	glBindVertexArray(0);
	quad = new Primitive(VBO, VAO, 4);
	verts.clear();

	//UI Quad
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	verts.push_back(Vertex(glm::vec3(0, 0, 0), glm::vec2(0, 0)));
	verts.push_back(Vertex(glm::vec3(0, -1, 0), glm::vec2(0, 1)));
	verts.push_back(Vertex(glm::vec3(1, -1, 0), glm::vec2(1, 1)));
	verts.push_back(Vertex(glm::vec3(1, 0, 0), glm::vec2(1, 0)));	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	glBindVertexArray(0);
	uiQuad = new Primitive(VBO, VAO, 4);
	verts.clear();
}