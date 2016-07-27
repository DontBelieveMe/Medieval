#pragma once
#include "../includes.h"
#include "../rendering/Shader.h"
#include "../rendering/Voxels.h"
#include "../ui/Font.h"
#include "State.h"

#include "../rendering/Renderer2D.h"
#include "../rendering/Texture.h"

class GameState : public State
{
private:
    ShaderProgram* modelShader;
	ShaderProgram* uiShader;
	Renderer2D*	   renderer2D;
    bool           showUI = true;

    Font* fontTest;

    Voxels*    vox;
    Model      ent;
    GLfloat    rot = 0;

public:

    GameState();

    void tick();

    void render();

    void destroy();

    ~GameState();

};
