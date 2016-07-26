/*
 * @author Barney Wilks
*/

#pragma once

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <unordered_map>
#include "includes.h"

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertPath, const std::string& fragPath);

    GLuint load(const std::string& vertPath, const std::string& fragPath);
    inline void use() { glUseProgram(this->program); }
    inline void halt() { glUseProgram(0); }

	GLint getUniformLoc(const std::string& name);
	void  uploadMatrix4f(GLint loc, const glm::mat4& matrix);
	void  uploadMatrix4f(const std::string& name, const glm::mat4& matrix);

private:
    GLuint createShader(const std::string& path, GLenum type, const std::string& errorMsg);
    void   checkError(GLuint element, bool isProgram, GLenum status, const std::string& errorMsg);
    void   validateProgram();

	std::unordered_map<std::string, GLint> uniformLocCache;
private:
    GLuint program;
};