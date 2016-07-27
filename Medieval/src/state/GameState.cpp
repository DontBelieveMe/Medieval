#include "GameState.h"

#include "../Input.h"

GameState::GameState()
{
    modelShader = new ShaderProgram("res/shaders/vert.shader", "res/shaders/frag.shader");
    modelShader->use();

    vox = new Voxels(1);
    ent = vox->loadModel("res/models/Ent.obj", "res/models/Ent.png");
    vox->setDrawingStage();

	uiShader = new ShaderProgram("res/shaders/vert2D.shader", "res/shaders/frag2D.shader");
	renderer2D = new Renderer2D(uiShader, "res/images/tileTest.png", glm::ivec2(2, 2)); //renderer2D will only use 1 texture, if multiple are needed, you need to stitch them together.
    /*                                                                            |
                                                                                  -------> If these are set to the width, and height of the texture, you can draw from the texture per-pixel!
    */


	rot = 0;
}

void GameState::tick()
{
	if (Input::keyPressed(GLFW_KEY_U))
		showUI = !showUI;
    rot += 1.0;
}

void GameState::render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vox->bind();
	modelShader->use();
    modelShader->uploadMatrix4f("projection", glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.001f, 200.0f));
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0, -15, -40));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
    modelShader->uploadMatrix4f("model", model);
    drawModel(ent);
    vox->halt();
	modelShader->halt();


	if (showUI)
	{
		glDisable(GL_CULL_FACE);
        renderer2D->bind();
        renderer2D->drawTile(0, 0, 1, 1, 10, 10, 64, 64);
        renderer2D->drawTile(0, 0, 1, 1, 84, 10, 128, 64);
        renderer2D->drawTile(0, 0, 2, 2, 10, 200, 64, 64);
        renderer2D->drawTile(1, 0, 1, 2, 100, 200, 64, 128);

        unsigned int xOff = 0;

        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 2; x++)
            {
                renderer2D->drawTile(x, y, 1, 1, (GLfloat)xOff * 70.0f, 400, 64, 64);
                xOff += 1;
            }
        }

        renderer2D->halt();
	}
}

void GameState::destroy()
{
    modelShader->halt();
    modelShader->deleteProgram();
}

GameState::~GameState()
{
    delete vox;
    delete modelShader;
}