#pragma once
#include "Component.h"

#include <iostream>

struct TestComponent : Component
{
	virtual void create()
	{

	}

	virtual void update()
	{
		std::cout << "Hello world!" << std::endl;
	}

	int value = 20;
};

struct TestComponent2 : Component
{
	virtual void create()
	{

	}

	virtual void update()
	{
		std::cout << "Hello world! 2" << std::endl;
	}
};