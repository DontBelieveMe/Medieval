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
#include <introspection/IntrospectionManager.h>

#include <AssetData.h>
#include <components/Prefab.h>

#include "../game/menus/PauseMenu.h"

#include <bullet/btBulletDynamicsCommon.h>

Map map;
PauseMenu *pause_menu;

struct TestStruct
{
	int member = 20;
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

	pauseState = new PauseState();

	GameObject *player = factory->CreateGameObject<VoxelModelComponent, RigidBodyComponent>("player");

	RigidBodyComponent *player_rigidbody = player->GetComponentFast<RigidBodyComponent>();
	player_rigidbody->mass = 1;
	player_rigidbody->inertia = glm::vec3(0.4f, 0.4f, 0.4f);

	VoxelModelComponent *playerRender = player->GetComponentFast<VoxelModelComponent>();
	playerRender->model = &playerModel;
	player->transform.position = glm::vec3(0, -15, -40);

	GameObject *ground = factory->CreateGameObject<VoxelModelComponent, RigidBodyComponent>("ground");
	RigidBodyComponent *ground_rigidbody = ground->GetComponentFast<RigidBodyComponent>();
	ground_rigidbody->mass = 0;
	ground_rigidbody->inertia = glm::vec3(0, 0, 0);
	ground_rigidbody->bounds = glm::vec3(2, 6, 1);
	VoxelModelComponent *ground_render = ground->GetComponentFast<VoxelModelComponent>();
	ground_render->model = &playerModel;
	ground->transform.position = glm::vec3(0, -40, -40);

	RigidBodyComponent s;
	std::cout << TYPE_OF_PTR(ground_rigidbody)->name << std::endl;

	factory->InitAll();

	Prefab prefab("player");
	prefab.Deserialize();
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

		map.GenerateChunks(camera->position);
		std::cout << "Loaded chunks: " << map.chunks.size() << '\n';
		map.Tick();

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