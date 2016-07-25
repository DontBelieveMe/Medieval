#include "Voxels.h"

Model Voxels::loadModel(std::string objPath, std::string palettePath)
{
	int ypos = loadPixIntoVec(palettePath);
	return Model{ 0, 0 };
}

int Voxels::loadPixIntoVec(std::string palettePath)
{
	std::vector<GLubyte> lodeData;//data from lodepng
	unsigned int width, height;
	unsigned int err = lodepng::decode(lodeData, width, height, palettePath);
	if (err) {
		std::string err_text = lodepng_error_text(err);
		std::cerr << "Error loading image: " << palettePath << ": " << err_text << endl;
		exit(1);
	}

	if (height != 1)
		std::cerr << "Palette: " << palettePath << " is not one pixel tall!!" << endl;
	if (width != 256)
		std::cerr << "Palettes are supposed to be 256pix wide from MagicaVoxel, if this is wrong, contact Ned" << endl;

	for (int x = 0; x < lodeData.size(); x+=4)
	{
		GLubyte r = lodeData[x];
		GLubyte g = lodeData[x + 1];
		GLubyte b = lodeData[x + 2];
		GLubyte a = lodeData[x + 3];
		GLuint fullColor = r << 24 | g << 16 | b << 8 | a;
		pixels[(x / 4) + currentY * 256] = fullColor;
	}
}

void Voxels::setDrawingStage()
{
	loadingStage = false;
}
