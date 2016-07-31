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

struct TestComponent2 : Component
{
	virtual void update()
	{
		std::cout << "Hello world! 2" << std::endl;
	}
};