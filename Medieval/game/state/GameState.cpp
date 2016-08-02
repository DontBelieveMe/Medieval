#include "GameState.h"

#include <Input.h>
#include "OpenAL/al.h"

#include <physics/Physics.h>
#include <Map.h>
#include <Utils.h>
#include <chrono>
#include <rendering/Primitives.h>
#include <components/GameObject.h>
#include <components/TestComponent.h>

Map map;

GameState::GameState()
{
    modelShader = new ShaderProgram("res/shaders/vert.shader", "res/shaders/frag.shader");
    modelShader->use();

    vox = new Voxels(1);
    ent = vox->loadModel("res/models/player.obj", "res/models/player.png");
    vox->setDrawingStage();

	uiShader = new ShaderProgram("res/shaders/vert2D.shader", "res/shaders/frag2D.shader");
	renderer2D = new Renderer2D(uiShader, "res/images/hud/sheet.png", glm::ivec2(8, 8)); //renderer2D will only use 1 texture, if multiple are needed, you need to stitch them together.
    /*                                                                            |
                                                                                  -------> If these are set to the width, and height of the texture, you can draw from the texture per-pixel!
    */
    fontTest = new Font(uiShader, "res/images/font.png");

	rot = 0;
	audioSystem = new AudioSystem();
	camera = new FreeCamera();

	Input::setMouseMode(Input::MouseMode::locked);

	map.addChunk({0,0});
	map.chunks[{0,0}].debugGenerate();

	GameObject object;
	object.addComponent<TestComponent>();
	object.addComponent<TestComponent>();
	std::cout << std::boolalpha << object.hasComponent<TestComponent>() << std::endl;
	object.removeComponent<TestComponent>();
	std::cout << std::boolalpha << object.hasComponent<TestComponent>() << std::endl;
}

int counter = 0;
int texIndex = 0;

bool hidden = true;
void GameState::tick()
{
	counter++;
	if (counter % 60 == 0) {
		texIndex++;
		if (texIndex == 5)
			texIndex = 0;
		counter = 0;
	}

	audioSystem->tick();

	if (Keys::toggle_ui.pressed())
		showUI = !showUI;

    rot += 1.0;
	camera->tick();

	if (Keys::toogle_focus.pressed())
	{
		if (hidden)
		{
			Input::setMouseMode(Input::MouseMode::normal);
			hidden = false;
		}
		else {
			Input::setMouseMode(Input::MouseMode::locked);
			hidden = true;
		}
	}
}

void GameState::render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vox->bind();
	modelShader->use();
    modelShader->uploadMatrix4f("projection", glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.001f, 200.0f));
	glm::mat4 view = camera->createView();
	modelShader->uploadMatrix4f("view", view);
	glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0, -15, -40));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
    modelShader->uploadMatrix4f("model", model);
    drawModel(ent);

	map.chunks[{0,0}].debugRender(view);

	if (showUI)
	{
	    renderer2D->bind();
		renderer2D->drawTile(0, 0, 2, 1, 10, 10, 64 * 3, 32 * 3);
		renderer2D->drawTile(texIndex, 1, 1, 1, 34 * 3, 12 * 3, 32 * 3, 32 * 3);
		renderer2D->drawTile(2, 0, 1, 1, 64 * 3 + 30, 10, 32 * 3, 32*3);
		std::stringstream ss;
		ss << Application::getInstance().FPS();

        uiShader->use();
        fontTest->bind();
		fontTest->drawString(ss.str() + " fps", WIDTH - 135, -35, 2);
		fontTest->drawString("Press P (def.) to toggle sound!", 220, HEIGHT - 170, 2.0);
		fontTest->drawString("Press U (def.) to toggle the UI!", 220, HEIGHT - 120, 2.0);
	}
}

void GameState::destroy()
{
	audioSystem->destroy();
    modelShader->deleteProgram();
}

GameState::~GameState()
{
	delete camera;
	delete audioSystem;
    delete vox;
    delete modelShader;
}