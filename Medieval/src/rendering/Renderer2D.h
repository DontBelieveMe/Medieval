/*
 * @author: Barney Wilks
 * @Edited: Ned Boulter
*/
#pragma once

#include "../includes.h"
#include "AdditionalVAO.h"
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
		static GLint result;
		glGetIntegerv(GL_CULL_FACE, &result);
		if (result == GL_TRUE)
			glDisable(GL_CULL_FACE);
		glGetIntegerv(GL_DEPTH_TEST, &result);
		if (result == GL_TRUE)
			glDisable(GL_DEPTH_TEST);

        shaderRef->use();
        AdditionalVAO::bind();
        tex->bind();
    }

    inline void halt()
    {
        //shaderRef->halt();
        //AdditionalVAO::halt();
        //tex->halt();
    }

    ~Renderer2D();

private:
    const glm::ivec2 sizeInTiles;
	int    count;
    Texture* tex;
	ShaderProgram *shaderRef;

};