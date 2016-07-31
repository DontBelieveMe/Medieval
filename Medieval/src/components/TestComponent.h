#pragma once
#include "Component.h"

#include <iostream>

struct TestComponent : Component
{
	virtual void update()
	{
		std::cout << "Hello world!" << std::endl;
	}
};