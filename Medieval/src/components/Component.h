#pragma once

struct Component
{
	virtual void create() = 0;
	virtual void update() = 0;
};