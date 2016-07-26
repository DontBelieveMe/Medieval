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
	~Texture();

	void destroy();
	inline void bind() { glBindTexture(GL_TEXTURE_2D, texId); }
	inline void halt() { glBindTexture(GL_TEXTURE_2D, 0); }

	inline unsigned int getWidth() { return width; }
	inline unsigned int getHeight() { return height; }
private:
	GLuint texId;
	unsigned int width, height;
};

