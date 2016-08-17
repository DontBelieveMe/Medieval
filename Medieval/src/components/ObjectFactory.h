#pragma once
#include <unordered_map>

#include "Component.h"
#include "GameObject.h"

#include "components/VoxelModelComponent.h"

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
		objects[id] = GameObject();
		return &objects[id];
	}

	// [Warning]: does no checking to see if the object exists
	GameObject *GetGameObject(const std::string& id)
	{
		return &objects.at(id);
	}

	void UpdateAll() 
	{
		for (auto& obj : objects)
		{
			obj.second.Update();
		}
	}

	void InitAll()
	{
		for (auto& obj : objects)
		{
			obj.second.Init();
		}
	}

	void RenderNecessary(ShaderProgram *model_shader, const glm::mat4& view)
	{
		model_shader->Use();
		model_shader->UploadMatrix4f("projection", Application::projection_matrix);
		model_shader->UploadMatrix4f("view", view);

		for (auto& obj : objects)
		{
			// Render all voxel models
			if (obj.second.HasComponent<VoxelModelComponent>())
			{
				VoxelModelComponent *voxel = obj.second.GetComponent<VoxelModelComponent>();
				voxel->Render(&obj.second, view, model_shader);
			}
		}
	}

	std::unordered_map<std::string, GameObject>& GetObjectsMap() { return objects; }
};