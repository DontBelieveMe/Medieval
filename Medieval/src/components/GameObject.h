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
	Component *getStaticComponent() 
	{
		static T t;
		return &t;
	}
public:
	GameObject();

	void update();

	template <typename T>
	void addComponent()
	{
		auto component = getStaticComponent<T>();
		components.push_back(component);
	}

	template <typename T>
	bool hasComponent()
	{
		bool exists = false;
		Component *t = getStaticComponent<T>();
		if (std::find(components.begin(), components.end(), t) != components.end()) {
			exists = true;
		}
		return exists;
	}

	template <typename T>
	T *getComponent()
	{
		if (!hasComponent<T>())
			return NULL;
		else
		{
			T *component = dynamic_cast<T*>(getStaticComponent<T>());
			return component;
		}
	}
};