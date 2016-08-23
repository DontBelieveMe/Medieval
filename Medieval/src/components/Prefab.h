#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


#include "../introspection/Serializable.h"
#include"../AssetData.h"
#include "../Utils.h"

#include "GameObject.h"
#include "components/RigidBodyComponent.h"
#include "components/VoxelModelComponent.h"

#include "../rendering/Voxels.h"


class Prefab : public Serializable, public GameObject
{
private:
	std::string name;

public:
	Prefab(const std::string& name)
		: name(name) {}

	// 
	virtual void Deserialize()
	{
		Voxels *vox = new Voxels(2);
		static Model playerModel = vox->loadModel(MODEL_PATH("player"));
		static Model ruu = vox->loadModel(MODEL_PATH("wilk"));

		using namespace rapidjson;
		std::string filename = AssetData::DataPath + name + AssetData::detail::DefaultDataExtension;
		io::File file(filename.c_str());

		const char *cstr_name = name.c_str();

		Document document;
		document.Parse(file.data);
		Value& position = document[cstr_name]["position"];
		Value& scale    = document[cstr_name]["scale"];
		Value& components = document[cstr_name]["components"];
		
		transform.position = glm::vec3(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
		transform.scale = glm::vec3(scale[0].GetFloat(), scale[1].GetFloat(), scale[2].GetFloat());

		for (Value::ConstMemberIterator it = components.MemberBegin(); it != components.MemberEnd(); ++it)
		{
			int ID = StrToInt(it->name.GetString());
			AddComponent(ID);
			if (ID == RigidBodyComponent::static_id) {
				float mass = components[it->name]["mass"].GetFloat();
				Value& inertia_value = components[it->name]["inertia"];
				glm::vec3 inertia = glm::vec3(inertia_value[0].GetFloat(), inertia_value[1].GetFloat(), inertia_value[2].GetFloat());
				RigidBodyComponent *component = GetComponentFast<RigidBodyComponent>();
				component->mass = mass;
				component->inertia = inertia;
			}
			if (ID == VoxelModelComponent::static_id)
			{
				VoxelModelComponent *component = GetComponentFast<VoxelModelComponent>();
				const char *modelPath = components[it->name]["modelName"].GetString();
				component->model = &playerModel;
			}
		}
	}

	virtual void Serialize()
	{
		
	}
};