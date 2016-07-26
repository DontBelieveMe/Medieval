#include "Texture.h"

#include "../extern/lodepng.h"

Texture::Texture(const std::string& path)
{
	std::vector<GLubyte> data;
	unsigned int error;
	error = lodepng::decode(data, width, height, path);
	if (error) {
		std::string errorMsg = lodepng_error_text(error);
		std::cout << "LodePNG Error: " << errorMsg << std::endl;
		system("pause"); exit(1);
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
}

void Texture::destroy()
{
	glDeleteTextures(1, &texId);
}