#pragma once

#include <unordered_map>

#define SERIALIZABLE(T) \
	virtual Serializable *CreateObject(){ return new T; }

class Serializable;

class SerializationManager
{
	typedef std::unordered_map<const char*, Serializable*(*)()> serialization_type_map;
	serialization_type_map serialization_map;
public:
	static SerializationManager *Get() { static SerializationManager s; return &s; }
	template <typename T>
	void MapInstance(const char *name, Serializable *s, Serializable *(T::*function)())
	{
		serialization_type_map::const_iterator it = serialization_map.find(name);
		if (it != serialization_map.end())
			return;

		serialization_map[name] = function;
	}
};

class Serializable
{
public:
	virtual void Serialize() {};
	virtual void Deserialize() {};

	virtual Serializable *CreateObject() { return NULL; }
};