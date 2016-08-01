#include "Shader.h"

#include <stdio.h>
#include <iostream>

const ShaderProgram *detail::currentShader = NULL;

ShaderProgram::ShaderProgram(const std::string& vertPath, const std::string& fragPath)
{
    this->program = load(vertPath, fragPath);
}

GLuint ShaderProgram::load(const std::string& vertPath, const std::string& fragPath)
{
    GLuint program = glCreateProgram();
    GLuint vert = createShader(vertPath, GL_VERTEX_SHADER, "Error in vert shader: ");
    GLuint frag = createShader(fragPath, GL_FRAGMENT_SHADER, "Error in frag shader: ");
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    checkError(program, true, GL_LINK_STATUS, "Error while linking program: ");
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

GLuint ShaderProgram::createShader(const std::string& path, GLenum type, const std::string& errorMsg)
{
    GLuint shader = glCreateShader(type);
    FILE *file = fopen(path.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *string = new char[fileSize + 1];
    fread(string, fileSize, 1, file);
    fclose(file);
    string[fileSize] = 0;
    const char *source [] = { string };
    glShaderSource(shader, 1, source, NULL);
    glCompileShader(shader);
    this->checkError(shader, false, GL_COMPILE_STATUS, errorMsg);
    delete [] string;
    string = NULL;
    return shader;
}

void ShaderProgram::checkError(GLuint element, bool isProgram, GLenum status, const std::string& errorMsg)
{
    GLint result;
    if (isProgram)
        glGetProgramiv(element, status, &result);
    else
        glGetShaderiv(element, status, &result);
    if (result == GL_FALSE)
    {
        GLint logLen;
        if (isProgram)
            glGetProgramiv(element, GL_INFO_LOG_LENGTH, &logLen);
        else
            glGetShaderiv(element, GL_INFO_LOG_LENGTH, &logLen);
        char *log = new char[logLen];
        if (isProgram)
            glGetProgramInfoLog(element, logLen, NULL, log);
        else
            glGetShaderInfoLog(element, logLen, NULL, log);
        std::cout << errorMsg << log << std::endl;
        delete[] log;
        system("pause");
        exit(1);
    }
}

void ShaderProgram::validateProgram()
{
    glValidateProgram(this->program);
}

void ShaderProgram::use()
{
	if (detail::currentShader != this) 
	{
		detail::currentShader = this; 
		glUseProgram(this->program);
	}
}

void ShaderProgram::halt()
{
	detail::currentShader = NULL;  
	glUseProgram(0);
}

GLint ShaderProgram::getUniformLoc(const std::string& name)
{
	std::unordered_map<std::string, GLint>::const_iterator it = uniformLocCache.find(name);
	if (it != uniformLocCache.end())
	{
		return it->second;
	}
	else {
		GLint loc = glGetUniformLocation(this->program, name.c_str());
		uniformLocCache[name] = loc;
		if (loc == -1) {
			std::cout << "Error: Uniform " << name << " does not exist!" << std::endl;
			// Goto is evil (but here it is slighly better than multiple return points
			goto errorGettingLoc;
		}
		return loc;
	}

	errorGettingLoc:
	return -1;
}


void  ShaderProgram::uploadMatrix4f(GLint loc, const glm::mat4& matrix)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(matrix[0][0]));
}

void ShaderProgram::uploadVector2f(const std::string& name, const glm::vec2& vec2)
{
	glUniform2f(getUniformLoc(name), vec2.x, vec2.y);
}

void ShaderProgram::uploadVector3f(const std::string& name, const glm::vec3& vec3)
{
	glUniform3f(getUniformLoc(name), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::uploadVector4f(const std::string& name, const glm::vec4& vec4)
{
	glUniform4f(getUniformLoc(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderProgram::uploadMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	uploadMatrix4f(getUniformLoc(name), matrix);
}

void ShaderProgram::deleteProgram()
{
    glDeleteProgram(program);
}