/*
*@author Barney Wilks
*/

#pragma once

#include <algorithm>
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
	void init();

	template <typename T>
	void addComponent()
	{
		// No need for duplicate components
		if (hasComponent<T>())
			return;

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

	template <typename T>
	void removeComponent()
	{
		// Cannot remove component that doesn't exist
		if (!hasComponent<T>())
			return;

		Component *component = getStaticComponent<T>();
		components.erase(std::remove(components.begin(), components.end(), component), components.end());
	}
};