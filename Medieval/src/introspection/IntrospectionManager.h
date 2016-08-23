#pragma once

#include <unordered_map>
#include <functional>

#include "TypeInfo.h"
#include "../Utils.h"
#include "Serializable.h"

#define REGISTER_TYPE(T) \
	IntrospectionManager::Get()->RegisterType<T>(#T, sizeof(T))

#define TYPE_INFO(T) \
	IntrospectionManager::Get()->StaticType<T>()

#define TYPE_OF(VAR) \
	IntrospectionManager::Get()->GetType<decltype(VAR)>()

#define TYPE_OF_PTR(PTR) \
	IntrospectionManager::Get()->StaticType< detail::RemovePointer< decltype(PTR) >::type >()

#define TYPE_INFO_STR(str) \
	IntrospectionManager::Get()->GetTypeByStr(str)	

#define TYPE_OF_MEMBER(T, MEMBER) \
	TYPE_INFO(detail::RemoveMemberDetail<decltype(&T::MEMBER)>::type)

#define REGISTER_MEMBER(T, MEMBER) \
	IntrospectionManager::Get()->RegisterMember(TYPE_INFO(T), TYPE_OF_MEMBER(T, MEMBER), #MEMBER)

#define SET_SERIALIZABLE(T) \
	IntrospectionManager::Get()->MapInstance<T>(#T)

#define CREATE_SERIALIZABLE_OBJECT(Name, STR_NAME) \
	dynamic_cast<Name*>(IntrospectionManager::Get()->GetSerializableObject(#STR_NAME))

namespace detail
{
	template <typename T>
	struct RemoveMemberDetail;

	template <typename T, typename M>
	struct RemoveMemberDetail<T M::*>
	{
		typedef T type;
	};

	template <typename T>
	struct RemovePointer 
	{ 
		typedef T type; 
	};
	
	template <typename T>
	struct RemovePointer<T*> 
	{ 
		typedef typename RemovePointer<T>::type type; 
	};
}

template <typename T>
struct Type { enum { is_primitive = 0 }; };
	
template <>
struct Type<int> { enum { is_primitive = 1 }; };
template <>
struct Type<bool> { enum { is_primitive = 1 }; };
template <>
struct Type<float> { enum { is_primitive = 1 }; };
template <>
struct Type<char> { enum { is_primitive = 1 }; };
template <>
struct Type<long> { enum { is_primitive = 1 }; };
template <>
struct Type<double> { enum { is_primitive = 1 }; };


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

	void RegisterMember(TypeInfo* out, TypeInfo *info, const char *name)
	{
		for (auto& member : out->members)
		{
			if (info == member.info && name == member.name)
			{
				return;
			}
		}

		out->members.push_back({ info, name });
	}

	
};