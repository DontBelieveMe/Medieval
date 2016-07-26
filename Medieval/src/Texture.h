/*
* @author Barney Wilks
*/

#pragma once

#include "includes.h"

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void destroy();
	inline void bind() { glBindTexture(GL_TEXTURE_2D, texId); }
private:
	GLuint texId;
	unsigned int width, height;

};

