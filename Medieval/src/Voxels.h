/*
* This class deals with all the voxel models.
* it also simultaneously raps a VAO object.
* this vao object contains all the data for various voxel models
* it can be destroyed and reinitialized with the voxel models in a certain stage (or whatever)

There are two stages, the loading stage (where you load voxel models), then the drawing stage.
Make sure you load in the loading stage, and draw in the drawing stage. If "bool loadingStage" is false
it IS the drawing stage.

* @author: Ned Boulter
*/
#pragma once

#include "includes.h"
#include "../extern/lodepng.h"
#include <vector>

#define PALETTE_WIDTH 256 //all magica voxel palettes are the same size
#define drawModel(model) glDrawArrays(GL_TRIANGLES, model.index, model.count)

struct Model;

class Voxels
{
private:
	bool loadingStage = true; //starts off true
	std::vector<GLfloat> data; //only contains data in loading stage
	GLuint vao, vbo; //this is for drawing stage

	std::vector<GLuint> pixels;//load stage texture
	int currentY = 0;
	GLuint texture; //draw stage texture

	int loadPixIntoVec(const std::string& palettePath); //returns the y-position

	Model loadObjData(const std::string& objPath);

public:

	Model loadModel(const std::string& objPath, const std::string& pallettePath);

	void setDrawingStage(); //after this is done, it cannot be undone.

};

struct Model
{
	int index;
	int count;
};
