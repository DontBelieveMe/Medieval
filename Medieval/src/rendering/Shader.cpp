#include "Shader.h"

#include <stdio.h>
#include <iostream>

#include "../Utils.h"

glm::mat4 perspective_matrix = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.005f, 200.0f);

const ShaderProgram *detail::current_shader = NULL;

ShaderProgram::ShaderProgram(const std::string& vert_path, const std::string& frag_path)
{
	this->program = Load(vert_path, frag_path);
}

GLuint ShaderProgram::Load(const std::string& vert_path, const std::string& frag_path)
{
    GLuint program = glCreateProgram();
    GLuint vert = CreateShader(vert_path, GL_VERTEX_SHADER, "Error in vert shader: ");
    GLuint frag = CreateShader(frag_path, GL_FRAGMENT_SHADER, "Error in frag shader: ");
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    CheckError(program, true, GL_LINK_STATUS, "Error while linking program: ");
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

GLuint ShaderProgram::CreateShader(const std::string& path, GLenum type, const std::string& error_msg)
{
    GLuint shader = glCreateShader(type);
	io::File file(path.c_str());
	glShaderSource(shader, 1, const_cast<const GLchar**>(&file.data), NULL);
	glCompileShader(shader);
	CheckError(shader, false, GL_COMPILE_STATUS, error_msg);
	return shader;
}

void ShaderProgram::CheckError(GLuint element, bool is_program, GLenum status, const std::string& error_msg)
{
    GLint result;
    if (is_program)
        glGetProgramiv(element, status, &result);
    else
        glGetShaderiv(element, status, &result);
    if (result == GL_FALSE)
    {
        GLint log_len;
        if (is_program)
            glGetProgramiv(element, GL_INFO_LOG_LENGTH, &log_len);
        else
            glGetShaderiv(element, GL_INFO_LOG_LENGTH, &log_len);
        char *log = new char[log_len];
        if (is_program)
            glGetProgramInfoLog(element, log_len, NULL, log);
        else
            glGetShaderInfoLog(element, log_len, NULL, log);
        std::cout << error_msg << log << std::endl;
        delete[] log;
        system("pause");
        exit(1);
    }
}

void ShaderProgram::ValidateProgram()
{
    glValidateProgram(this->program);
}

void ShaderProgram::Use()
{
	if (detail::current_shader != this)
	{
		detail::current_shader = this;
		glUseProgram(this->program);
	}
}

void ShaderProgram::Halt()
{
	detail::current_shader = NULL;
	glUseProgram(0);
}

GLint ShaderProgram::GetUniformLoc(const std::string& name)
{
	std::unordered_map<std::string, GLint>::const_iterator it = uniform_loc_cache.find(name);
	if (it != uniform_loc_cache.end())
	{
		return it->second;
	}
	else
    {
		GLint loc = glGetUniformLocation(this->program, name.c_str());
		uniform_loc_cache[name] = loc;
		if (loc == -1) {
			std::cout << "Error: Uniform " << name << " does not exist!" << std::endl;
			return -1;
		}
		return loc;
	}
}


void  ShaderProgram::UploadMatrix4f(GLint loc, const glm::mat4& matrix)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(matrix[0][0]));
}

void ShaderProgram::UploadVector2f(const std::string& name, const glm::vec2& vec2)
{
	glUniform2f(GetUniformLoc(name), vec2.x, vec2.y);
}

void ShaderProgram::UploadVector3f(const std::string& name, const glm::vec3& vec3)
{
	glUniform3f(GetUniformLoc(name), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::UploadVector4f(const std::string& name, const glm::vec4& vec4)
{
	glUniform4f(GetUniformLoc(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderProgram::UploadMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	UploadMatrix4f(GetUniformLoc(name), matrix);
}

void ShaderProgram::DeleteProgram()
{
    glDeleteProgram(program);
}