#pragma once
#include <unordered_map>

#include "Component.h"
#include "GameObject.h"

class ObjectFactory
{
private:
	std::unordered_map<std::string, GameObject> objects;
	template <typename T>
	GameObject& AddObjectComponent(GameObject& obj)
	{
		obj.AddComponent<T>();
		return obj;
	}

public:
	static ObjectFactory *Get()
	{
		static ObjectFactory object_factory;
		return &object_factory;
	}

	template <typename... Args>
	GameObject *CreateGameObject(const std::string& id)
	{
		GameObject object;
		[](...) {}((AddObjectComponent<Args>(object))...);
		objects[id] = object;
		return &objects[id];
	}
	
	GameObject *CreateGameObject(const std::string& id)
	{
		GameObject object;
		objects[id] = object;
		return &objects[id];
	}

	// [Warning]: does no checking to see if the object exists
	GameObject *GetGameObject(const std::string& id)
	{
		return &objects.at(id);
	}
};