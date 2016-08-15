/*
*@author Barney Wilks
*/

#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>

#include <glm/glm.hpp>

class GameObject;

#include "Component.h"

#define stringify(T) #T
#define ct(varT, T) (strcmp(#varT, #T))

class GameObject
{
private:
	std::vector<Component *> components;

	template <typename T>
	Component *GetStaticComponent()
	{
		for (Component *c : components)
		{
			if (ct(decltype(c), T))
			{
				return c;
			}
		}
		std::cout << "Creating new!" << std::endl;
		T *t = new T();
		return t;
	}

public:
	void DeleteAllComponents()
	{
		for (Component *component : components) { delete component; component = NULL; }
	}

	GameObject();
	
	void Update();
	void Init();
	
	template <typename T>
	void AddComponent()
	{
		// No need for duplicate components
		if (HasComponent<T>())
			return;

		auto component = GetStaticComponent<T>();
		components.push_back(component);
	}

	template <typename T>
	bool HasComponent()
	{
		bool exists = false;
		Component *t = GetStaticComponent<T>();
		if (std::find(components.begin(), components.end(), t) != components.end()) {
			exists = true;
		}
		return exists;
	}

	template <typename T>
	T *GetComponent()
	{
		if (!HasComponent<T>())
			return NULL;
		else
		{
			T *component = dynamic_cast<T*>(GetStaticComponent<T>());
			return component;
		}
	}

	template <typename T>
	void RemoveComponent()
	{
		// Cannot remove component that doesn't exist
		if (!HasComponent<T>())
			return;

		Component *component = GetStaticComponent<T>();
		components.erase(std::remove(components.begin(), components.end(), component), components.end());
		delete component;
		component = NULL;
	}

	inline int NumComponents() const 
	{
		return components.size();
	}

public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};