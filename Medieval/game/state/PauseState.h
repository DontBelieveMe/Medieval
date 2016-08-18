#pragma once

#include <state/State.h>

class PauseState : public State
{
public:
	PauseState();

	virtual void tick();

	virtual void render();

	virtual void destroy();
};