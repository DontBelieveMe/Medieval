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
}

void Renderer2D::drawTile(int tx, int ty, int tw, int th, int xOff, int yOff, int width, int height)
{
	shaderRef->uploadVector2f("tileWH", sizeInTiles);
	GLint tileLoc = shaderRef->getUniformLoc("tile");
    glUniform4f(tileLoc, tx, ty, tw, th);
	
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(xOff, yOff, 0.0f));
    model           = glm::scale(model, glm::vec3(width, height, 1.0f));
    shaderRef->uploadMatrix4f("model", model);
    DRAW_ADDITIONAL(STANDARD_QUAD);
	/*glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);                     i didn't know this had been changed to elements. The same can be done to AdditionalVAO if necessary :/
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
}

Renderer2D::~Renderer2D()
{
    delete tex;
}

void Renderer2D::destroy()
{
    tex->destroy();
}