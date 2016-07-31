/*
*@author Barney Wilks
*/

#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "Component.h"

class GameObject
{
private:
	std::vector<std::unique_ptr<Component>> components;

public:
	GameObject();

	void update();

	template <typename T>
	void addComponent()
	{
		auto component = std::make_unique<T>();
		components.push_back(std::move(component));
	}

	template <typename T>
	void hasComponent()
	{

	}
};