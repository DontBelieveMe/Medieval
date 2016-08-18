#pragma once

#include "PauseState.h"
#include "../extern/glad/glad.h"

PauseState::PauseState()
{

}

void PauseState::tick()
{

}

void PauseState::render()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void PauseState::destroy()
{

}