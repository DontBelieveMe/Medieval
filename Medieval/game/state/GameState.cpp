#include "GameState.h"
#include <Input.h>
#include "OpenAL/al.h"

#include <physics/Physics.h>
#include <Map.h>
#include <Utils.h>
#include <chrono>
#include <rendering/Primitives.h>
#include <components/GameObject.h>
#include <components/ObjectFactory.h>
#include <components/components/PhysicsComponent.h>
#include "../game/uiMenus/MenuExample.h"

Map map;
MenuExample *uiMenu;

GameState::GameState()
{
	Primitives::Init();

    modelShader = new ShaderProgram("res/shaders/vert.shader", "res/shaders/frag.shader");
    modelShader->Use();

    vox = new Voxels(2);
	playerModel = vox->loadModel("res/models/player.obj", "res/models/player.png");
	ruu = vox->loadModel("res/models/Ruu.obj", "res/models/Ruu.png");
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

	Input::SetMouseMode(Input::MouseMode::locked);

	uiMenu = new MenuExample(false);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ObjectFactory *factory = ObjectFactory::Get();
	//
	//GameObject *otherPlayer = factory->CreateGameObject<VoxelModelComponent>("otherPlayer");
	//VoxelModelComponent *otherRender = otherPlayer->GetComponent<VoxelModelComponent>();
	//otherRender->model = &ruu;
	//otherRender->object_shader = modelShader;
	//otherPlayer->position = glm::vec3(20, -15, -40);

	GameObject *player = factory->CreateGameObject<VoxelModelComponent>("player");
	VoxelModelComponent *playerRender = player->GetComponent<VoxelModelComponent>();
	playerRender->model = &playerModel;
	
	playerRender->object_shader = modelShader;
	player->position = glm::vec3(0, -15, -40);
	factory->InitAll();
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

	if (Keys::toggle_ui.Pressed())
		showUI = !showUI;

    rot += 1.0;
	camera->tick();

	uiMenu->Tick();


	if (Input::MouseButtonDown(1) && !map.ChunkExists(map.GetChunkPosForBlock(camera->position)))
	    map.GenerateChunk(map.GetChunkPosForBlock(camera->position));


	static bool wireframe;
	if (Input::MouseButtonPressed(2))
	    glPolygonMode(GL_FRONT_AND_BACK, (wireframe = !wireframe) ? GL_LINE : GL_FILL);

	ObjectFactory *factory = ObjectFactory::Get();
	factory->UpdateAll();
}

void GameState::render()
{
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Cache this as a class member
	ObjectFactory *factory = ObjectFactory::Get();

	glm::mat4 view = camera->createView();
    vox->bind();
	factory->RenderNecessary(view);
 //   
	//modelShader->UploadMatrix4f("projection", Application::projection_matrix);
	//modelShader->UploadMatrix4f("view", view);
	//glm::mat4 model = glm::mat4(1.0);
 //   model = glm::translate(model, glm::vec3(0, -15, -40));
	//model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
 //   modelShader->UploadMatrix4f("model", model);
 //   DrawModel(ent);

	map.Render(view, {0,0,0});

	glDisable(GL_DEPTH_TEST);

	if (showUI)
	{
	    renderer2D->bind();
		renderer2D->drawTile(0, 0, 2, 1, 10, 10, 64 * 3, 32 * 3);
		renderer2D->drawTile(texIndex, 1, 1, 1, 34 * 3, 12 * 3, 32 * 3, 32 * 3);
		renderer2D->drawTile(2, 0, 1, 1, 64 * 3 + 30, 10, 32 * 3, 32*3);

        uiShader->Use();
        fontTest->bind();
		fontTest->drawString(Jo(Application::GetInstance().FPS(), " fps"), WIDTH - 135, -35, 2);
		fontTest->drawString("Press P (def.) to toggle sound!", 220, HEIGHT - 170, 2.0);
		fontTest->drawString("Press U (def.) to toggle the UI!", 220, HEIGHT - 120, 2.0);
	}

	uiMenu->Render();

	glEnable(GL_CULL_FACE);
}

void GameState::destroy()
{
	audioSystem->destroy();
    modelShader->DeleteProgram();
}

GameState::~GameState()
{
	delete camera;
	delete audioSystem;
    delete vox;
    delete modelShader;
}