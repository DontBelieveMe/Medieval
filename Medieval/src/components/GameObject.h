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
#include "../Utils.h"

#define ct(varT, T) (strcmp(#varT, #T))
#define test(varT, T) std::cout << #varT << ", " << #T << std::endl;
#define strfy(T) #T

class GameObject
{
private:
	std::vector<Component *> components;

	template <typename T>
	Component *GetInternalComponent()
	{
		for (Component *c : components)
		{
			if (ct(decltype(c), T))
			{
				return c;
			}
		}
		return NULL;
	}

	template <typename T>
	Component *GetStaticComponent()
	{
		Component *existing = GetInternalComponent<T>();
		if (existing != NULL) {
			MSVC_LOG("Returning existing!");
			return existing;
		}
		
		MSVC_LOG("Creating new component!");

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
		auto component = GetStaticComponent<T>();
		components.push_back(component);
	}

	template <typename T>
	bool HasComponent()
	{
		Component *component = GetInternalComponent<T>();
		return component != NULL;
	}

	template <typename T>
	T *GetComponent()
	{
		if (!HasComponent<T>())
			return NULL;
		else
		{
			T *component = dynamic_cast<T*>(GetInternalComponent<T>());
			return component;
		}
	}

	template <typename T>
	void RemoveComponent()
	{
		// Cannot remove component that doesn't exist
		if (!HasComponent<T>())
			return;

		Component *component = GetInternalComponent<T>();
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