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
#include <components/components/RigidBodyComponent.h>
#include <components/components/VoxelModelComponent.h>
#include <introspection/IntrospectionManager.h>

#include <AssetData.h>
#include <components/Prefab.h>

#include "../game/menus/PauseMenu.h"

#include <bullet/btBulletDynamicsCommon.h>

Map map;
PauseMenu *pause_menu;

struct TestStructBase
{
	int base = 200;
};

struct tPlayer
{
	glm::vec3 position;
	glm::vec3 scale;
};

GameState::GameState()
{
	Primitives::Init();

    modelShader = new ShaderProgram(SHADER_PATH("vert", "frag"));
    modelShader->Use();

    vox = new Voxels(2);
	playerModel = vox->loadModel(MODEL_PATH("player"));
	ruu = vox->loadModel(MODEL_PATH("wilk"));

	vox->setDrawingStage();

	uiShader = new ShaderProgram(SHADER_PATH("vert2D", "frag2D"));
	renderer2D = new Renderer2D(uiShader, TEXTURE_PATH("hud/sheet"), glm::ivec2(8, 8)); //renderer2D will only use 1 texture, if multiple are needed, you need to stitch them together.
    /*                                                                            |
                                                                                  -------> If these are set to the width, and height of the texture, you can draw from the texture per-pixel!
    */
    fontTest = new Font(uiShader, TEXTURE_PATH("font"));

	rot = 0;
	audioSystem = new AudioSystem();
	camera = new FreeCamera();

	Input::SetMouseMode(Input::MouseMode::locked);

	pause_menu = new PauseMenu(false);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ObjectFactory *factory = ObjectFactory::Get();

	GameObject *player = factory->CreateGameObjectFromPrefab("player");
	GameObject *ground = factory->CreateGameObjectFromPrefab("ground");

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
	pause_menu->Tick();


	if (!pause_menu->enabled)
	{
		if (Keys::toggle_ui.Pressed())
			showUI = !showUI;

		camera->tick();

	// Sorry egor - it was freaking my FPS out sooo much
	//	map.GenerateChunks(camera->position);
	//	std::cout << "Loaded chunks: " << map.chunks.size() << '\n';
	//	map.Tick();

		static bool wireframe;
		if (Input::MouseButtonPressed(2))
			glPolygonMode(GL_FRONT_AND_BACK, (wireframe = !wireframe) ? GL_LINE : GL_FILL);

		ObjectFactory *factory = ObjectFactory::Get();
		GameObject *player = factory->GetGameObject("player");
		if (Input::KeyPressed(GLFW_KEY_L))
		{
			RigidBodyComponent *rigidBodyComponent = player->GetComponentFast<RigidBodyComponent>();
			rigidBodyComponent->AddForce(glm::vec3(0, 1000, 0));
		}
		factory->UpdateAll();

	}

}

void GameState::render()
{
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Cache this as a class member
	ObjectFactory *factory = ObjectFactory::Get();

	glm::mat4 view = camera->createView();
    vox->bind();
	factory->RenderNecessary(modelShader, view);

	map.Render(view, {0,-70,0}, camera->position);

	glDisable(GL_DEPTH_TEST);

	if (pause_menu->enabled)
	{
		uiShader->Use();
		fontTest->bind();
		fontTest->drawString("resume button out of order. TODO: Fix", 150, 100, 2.f);
	}

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

	pause_menu->Render();

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