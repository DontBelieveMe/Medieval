/*
 @author: Ned Boulter

 I did not want this vao to be packaged into Renderer2D, since this will allow us to create
 multiple instances of Renderer2D (that essentially deal with different textures).

 It also means, that we can have hand-coded models, that are additional to the voxel models.
 (E.G, if you needed a cube to test a collision etc...)

 all models should be written directly into the array, and can be defined, as shown
  */

#pragma once
#include "../includes.h"

#define STANDARD_QUAD 0, 6 // for 2D rendering
#define DRAW_ADDITIONAL(add) glDrawArrays(GL_TRIANGLES, add)

class AdditionalVAO
{
private:

    static GLfloat* getData(GLsizei & size, std::vector<GLushort>& indices); //this should return a copy, do not change to a ref

public:
    static GLuint vao;
    static GLuint vbo;
	static GLuint ibo;

    static void init();

    static void bind();

    static void halt();

    static void destroy();
};