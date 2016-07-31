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
	std::vector<Component *> components;

	template <typename T>
	Component *staticComponent() 
	{
		static T t;
		return &t;
	}
public:
	void update();

	template <typename T>
	void addComponent()
	{
		auto component = staticComponent<T>();
		components.push_back(component);
	}

	template <typename T>
	bool hasComponent()
	{
		Component *t = staticComponent<T>();
		if (std::find(components.begin(), components.end(), t) != components.end()) {
			return true;
		}
		return false;
	}
};