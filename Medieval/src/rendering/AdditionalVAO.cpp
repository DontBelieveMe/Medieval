#include "AdditionalVAO.h"

//static variable declarations
GLuint AdditionalVAO::vao = 0;
GLuint AdditionalVAO::vbo = 0;

GLfloat* AdditionalVAO::getData(GLsizei & size)
{
    float dataArray[] = 
    {
        //THIS IS WHERE THE DATA MUST BE HAND-CODED

        //STANDARD_QUAD index: 0, count: 6
        0.0, 0.0, 0.0,      0.0, 0.0,   0, 0, 0,
        1.0, 0.0, 0.0,      1.0, 0.0,   0, 0, 0,
        1.0, 1.0, 0.0,      1.0, 1.0,   0, 0, 0,

        0.0, 0.0, 0.0,      0.0, 0.0,   0, 0, 0,
        0.0, 1.0, 0.0,      0.0, 1.0,   0, 0, 0,
        1.0, 1.0, 0.0,      1.0, 1.0,   0, 0, 0,


    };

    size = sizeof(dataArray);
    return dataArray;
}

void AdditionalVAO::init()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLsizei size;
    GLfloat* data = getData(size);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_ATTRIB);
    glEnableVertexAttribArray(TEXTURE_ATTRIB);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(0));
    glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 5));
    glBindVertexArray(0);
}

void AdditionalVAO::bind()
{
    glBindVertexArray(vao);
}

void AdditionalVAO::halt()
{
    glBindVertexArray(0);
}

void AdditionalVAO::destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
