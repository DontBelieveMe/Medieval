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
	Renderer2D(ShaderProgram *shader, const std::string & tex_path, glm::ivec2 size_in_tiles, glm::mat4 &ortho);
	Renderer2D(ShaderProgram *shader, const std::string & tex_path, glm::ivec2 size_in_tiles);

    void drawTile(int uv_x, int uv_y, int uv_w, int uv_h, int x_offset, int y_offset, int width, int height);
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

        shader_ptr->Use();
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
    const glm::ivec2 size_in_tiles;
	int    count;
    Texture* tex;
	ShaderProgram *shader_ptr;

};