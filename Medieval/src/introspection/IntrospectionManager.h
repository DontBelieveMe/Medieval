#pragma once

#include <unordered_map>
#include <functional>

#include "TypeInfo.h"
#include "../Utils.h"
#include "Serializable.h"



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

	template <typename T, typename U>
	constexpr size_t OffsetOf(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}
}

template <typename T>
struct Type { enum { is_fundemental = false, is_pointer = false }; };

template <typename T>
struct Type<T*> { enum { is_pointer = true, is_fundemental = false
}; };

template <>
struct Type<int> { enum {
	is_fundemental = true, is_pointer = false
}; };
template <>
struct Type<bool> { enum {
	is_fundemental = true, is_pointer = false
}; };
template <>
struct Type<float> { enum {
	is_fundemental = true, is_pointer = false
}; };
template <>
struct Type<char> { enum {
	is_fundemental = true, is_pointer = false
}; };
template <>
struct Type<long> { enum {
	is_fundemental = true, is_pointer = false
}; };
template <>
struct Type<double> { enum {
	is_fundemental = true, is_pointer = false
}; };


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

	void RegisterMember(TypeInfo* out, TypeInfo *info, const char *name, size_t offset)
	{
		for (auto& member : out->members)
		{
			if (info == member.info && name == member.name)
			{
				return;
			}
		}

		out->members.push_back({ info, name , offset });
	}
};