#pragma once

#include <unordered_map>

#include "TypeInfo.h"

#define REGISTER_TYPE(T) \
	IntrospectionManager::Get()->RegisterType<T>(#T, sizeof(T))

#define TYPE_INFO(T) \
	IntrospectionManager::Get()->StaticType<T>()

#define TYPE_OF(VAR) \
	IntrospectionManager::Get()->StaticType<decltype(VAR)>()

#define TYPE_INFO_STR(str) \
	IntrospectionManager::Get()->GetTypeByStr(str)	

class IntrospectionManager
{
private:
	std::unordered_map<const char *, TypeInfo*> type_map;

public:
	static IntrospectionManager *Get()
	{
		static IntrospectionManager meta;
		return &meta;
	}

	TypeInfo *GetTypeByStr(const char *type_name)
	{
		return type_map.at(type_name);
	}

	template <typename T>
	TypeInfo *StaticType()
	{
		static TypeInfo t;
		return &t;
	}

	template <typename T>
	TypeInfo *RegisterType(const char *name, int size)
	{
		TypeInfo *type = StaticType<T>();
		type->name = name;
		type->size = size;
		type_map.insert(std::make_pair(name, type));
		return type;
	}
};