#include "Renderer2D.h"

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string & texPath, glm::ivec2 sizeInTiles, glm::mat4 &ortho)
    : shaderRef(shader), sizeInTiles(sizeInTiles)
{
    GLint previousShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);
	shader->use();
	shader->uploadMatrix4f("ortho", ortho);		// Probably shouldn't hardcode this name/location: TODO change
    glUseProgram(previousShader);
    tex = new Texture(texPath);
    createVertexArray();
}

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string& texPath, glm::ivec2 sizeInTiles)
	: shaderRef(shader), sizeInTiles(sizeInTiles)
{
    GLint previousShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);
	shader->use();
	shader->uploadMatrix4f("ortho", glm::ortho(0.f, (float)WIDTH, (float)HEIGHT, 0.0f, 1.0f, -1.0f));
    glUseProgram(previousShader);
    tex = new Texture(texPath);
    createVertexArray();
}

void Renderer2D::createVertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint indices[] = 
	{
		0, 1, 2, 0, 4, 2 
	};
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float data[] = 
    {
        0.0, 0.0, 0.0,      0.0, 0.0,
        1.0, 0.0, 0.0,      1.0, 0.0,
        1.0, 1.0, 0.0,      1.0, 1.0,

        0.0, 0.0, 0.0,      0.0, 0.0,
        0.0, 1.0, 0.0,      0.0, 1.0,
        1.0, 1.0, 0.0,      1.0, 1.0,
    };

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    count = 6;
}

void Renderer2D::drawTile(int tx, int ty, int tw, int th, int xOff, int yOff, int width, int height)
{
	shaderRef->uploadVector2f("tileWH", sizeInTiles);
	GLint tileLoc = shaderRef->getUniformLoc("tile");
    glUniform4f(tileLoc, tx, ty, tw, th);
	
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(xOff, yOff, 0.0f));
    model           = glm::scale(model, glm::vec3(width, height, 1.0f));
    shaderRef->uploadMatrix4f("model", model);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Renderer2D::~Renderer2D()
{
    delete tex;
}

void Renderer2D::destroy()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
    tex->destroy();
}