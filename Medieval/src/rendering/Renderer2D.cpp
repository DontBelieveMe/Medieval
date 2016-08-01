#include "Renderer2D.h"

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string & texPath, glm::ivec2 sizeInTiles, glm::mat4 &ortho)
    : sizeInTiles(sizeInTiles), shaderRef(shader)
{
    GLint previousShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);
	shader->use();
	shader->uploadMatrix4f("ortho", ortho);		// Probably shouldn't hardcode this name/location: TODO change
    glUseProgram(previousShader);
    tex = new Texture(texPath);
}

Renderer2D::Renderer2D(ShaderProgram *shader, const std::string& texPath, glm::ivec2 sizeInTiles)
	: sizeInTiles(sizeInTiles), shaderRef(shader)
{
    GLint previousShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShader);
	shader->use();
	shader->uploadMatrix4f("ortho", glm::ortho(0.f, (float)WIDTH, (float)HEIGHT, 0.0f, 1.0f, -1.0f));
    glUseProgram(previousShader);
    tex = new Texture(texPath);
}

void Renderer2D::drawTile(int tx, int ty, int tw, int th, int xOff, int yOff, int width, int height)
{
	shaderRef->uploadVector2f("tileWH", sizeInTiles);
	GLint tileLoc = shaderRef->getUniformLoc("tile");
    glUniform4i(tileLoc, tx, ty, tw, th);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(xOff, yOff, 0.0f));
    model           = glm::scale(model, glm::vec3(width, height, 1.0f));
    shaderRef->uploadMatrix4f("model", model);
	glBindVertexArray(AdditionalVAO::vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AdditionalVAO::ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Renderer2D::~Renderer2D()
{
    delete tex;
}

void Renderer2D::destroy()
{
    tex->destroy();
}