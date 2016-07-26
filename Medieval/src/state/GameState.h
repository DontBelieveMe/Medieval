#pragma once
#include "../includes.h"
#include "../rendering/Shader.h"
#include "../rendering/Voxels.h"
#include "State.h"

class GameState : public State
{
private:
    ShaderProgram* modelShader;

    Voxels* vox;
    Model ent;

    GLfloat rot;

public:

    GameState();

    void tick();

    void render();

    void destroy();

    ~GameState();

};
