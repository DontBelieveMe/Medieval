/*
 * @author: Barney Wilks
 * @Edited: Ned Boulter
*/
#pragma once

#include "../includes.h"
#include "Texture.h"
#include "Shader.h"

class Renderer2D
{
public:
	Renderer2D(ShaderProgram *shader, const std::string & texPath, glm::ivec2 sizeInTiles, glm::mat4 &ortho);
	Renderer2D(ShaderProgram *shader, const std::string & texPath, glm::ivec2 sizeInTiles);
    
    void drawTile(int tx, int ty, int tw, int th, int xOff, int yOff, int width, int height);
	void destroy();

    inline void bind()
    {
        shaderRef->use();
        tex->bind();
        glBindVertexArray(vao);
    }

    inline void halt()
    {
        shaderRef->halt();
        tex->halt();
        glBindVertexArray(0);
    }

    ~Renderer2D();

private:
    const glm::ivec2 sizeInTiles;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	int    count;
    Texture* tex;
	ShaderProgram *shaderRef;

    void createVertexArray();
};