/*
* @author Barney Wilks
*/
/*
*	@author Barney Wilks
*/
#pragma once

#include "../includes.h"

class Texture
{
public:
	Texture(const std::string& path);
	Texture(const std::vector<GLubyte> &data);
	~Texture();

	inline void Bind() const { glBindTexture(GL_TEXTURE_2D, _texId); }
	inline void Halt() const { glBindTexture(GL_TEXTURE_2D, 0); }

	inline unsigned int GetWidth() const { return _width; }
	inline unsigned int GetHeight() const { return _height; }
private:
	GLuint _texId;
	unsigned int _width, _height;
};

