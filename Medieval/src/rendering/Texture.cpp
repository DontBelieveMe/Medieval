#include "Texture.h"

#include "../extern/lodepng.h"

Texture::Texture(const std::string& path)
{
	std::vector<GLubyte> data;
	unsigned int error;
	error = lodepng::decode(data, _width, _height, path);
	if (error) {
		std::string errorMsg = lodepng_error_text(error);
		std::cerr << "LodePNG Error: (" << path  << ") " << errorMsg << std::endl;
		system("pause"); exit(1);
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &_texId);
	glBindTexture(GL_TEXTURE_2D, _texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::vector<GLubyte> &data)
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &_texId);
	glBindTexture(GL_TEXTURE_2D, _texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texId);
}