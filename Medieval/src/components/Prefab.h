#pragma once

#include <rapidjson/rapidjson.h>

#include "../introspection/Serializable.h"
#include "GameObject.h"

class Prefab : public Serializable, public GameObject
{
public:
	Prefab();

	virtual void Deserialize()
	{
		
	}

	virtual void Serialize()
	{
		
	}
};