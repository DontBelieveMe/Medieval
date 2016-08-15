#pragma once
#include <includes.h>
#include <rendering/Shader.h>
#include <rendering/Voxels.h>
#include <ui/Font.h>
#include <state/State.h>

#include <rendering/Renderer2D.h>
#include <rendering/Texture.h>
#include <audio/AudioSystem.h>
#include <FreeCamera.h>

class GameState : public State
{
private:
    ShaderProgram* modelShader;
	ShaderProgram* uiShader;
	Renderer2D*	   renderer2D;
    bool           showUI = true;

    Font* fontTest;

    Voxels*    vox;
    Model      playerModel;
	Model	   ruu;
    GLfloat    rot = 0;
	FreeCamera *camera;
	AudioSystem *audioSystem;

public:

    GameState();

    void tick();

    void render();

    void destroy();

    ~GameState();

};
