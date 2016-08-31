#include "Prefab.h"



void Prefab::Deserialize()
{
	using namespace rapidjson;
	std::string filename = AssetData::DataPath + name + AssetData::detail::DefaultDataExtension;
	io::File file(filename.c_str());

	const char *cstr_name = name.c_str();

	Document document;
	document.Parse(file.data);
	Value& position = document[cstr_name]["position"];
	Value& scale = document[cstr_name]["scale"];
	Value& components = document[cstr_name]["components"];

	transform.position = glm::vec3(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	transform.scale = glm::vec3(scale[0].GetFloat(), scale[1].GetFloat(), scale[2].GetFloat());

	for (Value::ConstMemberIterator component_it = components.MemberBegin(); component_it != components.MemberEnd(); ++component_it)
	{
		int ID = StrToInt(component_it->name.GetString());
		AddComponent(ID);

		EXECUTE_ALL_SPECIALISED_COMPONENT_LOADING(ID, component_it);
	}
}

void Prefab::Serialize() { }