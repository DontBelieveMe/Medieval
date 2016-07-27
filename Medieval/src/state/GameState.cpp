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
	renderer2D = new Renderer2D(uiShader);
	float vertices[] = {
		0.f, 0.f,  0.f,			  1.0f, 0.0f, 0.0f,		0.f, 1.f,
		WIDTH, 0.f,    0.0f,		  0.0f, 1.0f, 0.0f,		1.f, 1.f,
		0,   HEIGHT,  0.0f,		  0.0f, 0.0f, 1.0f,		0.f, 0.0f
	};
	renderer2D->createVertexArray(vertices, sizeof(vertices));
	texture2D = new Texture("res/images/texture.png");

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
		texture2D->bind();
		renderer2D->draw();
		texture2D->halt();
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