/*
 * @author Barney Wilks
*/

#pragma once

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <unordered_map>
#include "../includes.h"

extern glm::mat4 perspective_matrix;

class ShaderProgram;

namespace detail
{
	extern const ShaderProgram *current_shader;
}

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vert_path, const std::string& frag_path);

    GLuint Load(const std::string& vert_path, const std::string& frag_path);
	void Use();
	void Halt();

	GLint GetUniformLoc(const std::string& name);

	void  UploadMatrix4f(GLint loc, const glm::mat4& matrix);
	void  UploadMatrix4f(const std::string& name, const glm::mat4& matrix);
	void  UploadVector2f(const std::string& name, const glm::vec2& vec2);
	void  UploadVector3f(const std::string& name, const glm::vec3& vec3);
	void  UploadVector4f(const std::string& name, const glm::vec4& vec4);

    void  DeleteProgram();

private:
    GLuint program;
    GLuint CreateShader(const std::string& path, GLenum type, const std::string& error_msg);
    void   CheckError(GLuint element, bool is_program, GLenum status, const std::string& error_msg);
    void   ValidateProgram();

	std::unordered_map<std::string, GLint> uniform_loc_cache;
};