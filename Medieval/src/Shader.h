#pragma once

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include "includes.h"

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertPath, const std::string& fragPath);
    
    GLuint load(const std::string& vertPath, const std::string& fragPath);
    inline void use() { glUseProgram(this->program); }
    inline void halt() { glUseProgram(0); }

private:
    GLuint createShader(const std::string& path, GLenum type, const std::string& errorMsg);
    void   checkError(GLuint element, bool isProgram, GLenum status, const std::string& errorMsg);
    void   validateProgram();

private:
    GLuint program;
};