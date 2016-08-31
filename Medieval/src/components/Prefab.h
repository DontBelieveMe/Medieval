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
#include "Common.h"


class Prefab : public Serializable, public GameObject
{
private:
	std::string name;

public:
	Prefab(const std::string& name): name(name) {}

	template <typename T>
	void DeserializeComponent(T* out, const rapidjson::Value::ConstMemberIterator& component_it)
	{
		for (rapidjson::Value::ConstMemberIterator it = component_it->value.MemberBegin(); it != component_it->value.MemberEnd(); ++it) 
		{
			const char *name = it->name.GetString();
			if (it->value.IsArray()) 
			{
				auto value = it->value.GetArray();
				SET_MEMBER_STR_OBJ_PTR(out, name, glm::vec3(value[0].GetFloat(), value[1].GetFloat(), value[2].GetFloat()));
			}
			else if (it->value.IsFloat() || it->value.IsInt()) 
			{
				float value = it->value.GetFloat();
				SET_MEMBER_STR_OBJ_PTR(out, name, value);
			}
			else if (it->value.IsString())
			{
				const char *value = it->value.GetString();
				SET_MEMBER_STR_OBJ_PTR(out, name, std::string(value));
			}
		}
	}

	virtual void Deserialize();
	virtual void Serialize();
};