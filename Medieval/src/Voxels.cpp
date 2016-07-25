#include "Voxels.h"

Model Voxels::loadModel(const std::string& objPath, const std::string& palettePath)
{
	loadPixIntoVec(palettePath);

	return Model{ 0, 0 };
}

Model Voxels::loadObjData(const std::string& objPath)
{
	
	return Model{ 0, 0 };
}

int Voxels::loadPixIntoVec(const std::string& palettePath)
{
	std::vector<GLubyte> lodeData;//data from lodepng
	unsigned int width, height;
	unsigned int err = lodepng::decode(lodeData, width, height, palettePath);
	if (err) {
		std::string err_text = lodepng_error_text(err);
		std::cerr << "Error loading image: " << palettePath << ": " << err_text << endl;
		system("pause");  exit(1);
	}

	if (height != 1)
		std::cerr << "Palette: " << palettePath << " is not one pixel tall!!" << endl;
	if (width != 256)
		std::cerr << "Palettes are supposed to be 256pix wide from MagicaVoxel, if this is wrong, contact Ned" << endl;

	for (unsigned int x = 0; x < lodeData.size(); x+=4)
	{
		GLubyte r = lodeData[x];
		GLubyte g = lodeData[x + 1];
		GLubyte b = lodeData[x + 2];
		GLubyte a = lodeData[x + 3];
		GLuint fullColor = r << 24 | g << 16 | b << 8 | a;
		pixels.push_back(fullColor);
	}

	return currentY;
}

void Voxels::setDrawingStage()
{
	loadingStage = false;
}
