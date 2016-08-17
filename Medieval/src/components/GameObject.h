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

class GameObject
{
private:
	std::vector<Component *> components;

	Component *GetInternalComponent(int id)
	{
		for (Component *component : components)
		{
			if (component->id == id)
				return component;
		}

		return NULL;
	}

	template <typename T>
	T *GetInternalComponent()
	{
		return dynamic_cast<T*>(GetInternalComponent(T::static_id));
	}


public:
	void DeleteAllComponents()
	{
		for (Component *component : components) { delete component; component = NULL; }
		components.clear();
	}

	GameObject();
	~GameObject() { DeleteAllComponents(); }
	
	void Update();
	void Init();
	
	template <typename T>
	void AddComponent()
	{
		Component *component = new T();
		component->id = T::static_id;
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