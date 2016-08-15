#pragma once

struct Component
{
	virtual void Create() = 0;
	virtual void Update(GameObject *object) = 0;
};