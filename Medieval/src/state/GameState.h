#pragma once
#include "../includes.h"
#include "../rendering/Shader.h"
#include "../rendering/Voxels.h"
#include "State.h"

#include "../rendering/Renderer2D.h"
#include "../rendering/Texture.h"

class GameState : public State
{
private:
    ShaderProgram* modelShader;
	ShaderProgram* uiShader;
	Renderer2D*	   renderer2D;
	Texture*	   texture2D;

    Voxels* vox;
    Model ent;

    GLfloat rot = 0;

	bool	showUI = false;

public:

    GameState();

    void tick();

    void render();

    void destroy();

    ~GameState();

};
