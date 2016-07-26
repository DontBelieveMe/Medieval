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

#include "../includes.h"
#include "../../extern/lodepng.h"

#include <vector>
#include <sstream>
#include <fstream>
#include <map>

#define PALETTE_WIDTH 256 //all magica voxel palettes are the same size
#define drawModel(model) glDrawArrays(GL_TRIANGLES, model.index, model.count);

struct Model;

class Voxels
{
private:
    const unsigned int numberOfModels;

	bool loadingStage = true; //starts off true
    int currentIndex = 0;
	std::vector<GLfloat> data; //only contains data in loading stage

	GLuint vao, vbo; //this is for drawing stage

	std::vector<GLuint> pixels;//load stage texture
	int currentY = 0;
	GLuint texture; //draw stage texture

	int loadPixIntoVec(const std::string & palettePath); //returns the y-position

	Model loadObjData(const std::string & objPath, int yPos);

    std::vector<std::string> splitStr(const std::string & toSplit, char splitter);

public:
    Voxels(const unsigned int numberOfModels); // due to an issue I ran into, you need to specify the amount of models you are going to load

	Model loadModel(const std::string& objPath, const std::string& pallettePath);

	void setDrawingStage(); //after this is done, it cannot be undone.

    inline void bind() { glBindVertexArray(vao); glBindTexture(GL_TEXTURE_2D, texture); }
    inline void halt() { glBindVertexArray(0); glBindTexture(GL_TEXTURE_2D, 0); };

    void destroy();
};


struct Model
{
	int index;
	int count;
};
