#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


#include "../introspection/Serializable.h"
#include"../AssetData.h"
#include "../Utils.h"

#include "GameObject.h"

class Prefab : public Serializable, public GameObject
{
private:
	std::string name;

public:
	Prefab(const std::string& name)
		: name(name) {}

	virtual void Deserialize()
	{
		using namespace rapidjson;
		std::string filename = AssetData::DataPath + name + AssetData::detail::DefaultDataExtension;
		io::File file(filename.c_str());

		Document document;
		document.Parse(file.data);
		
	}

	virtual void Serialize()
	{
		
	}
};