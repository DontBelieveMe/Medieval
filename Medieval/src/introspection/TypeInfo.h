#pragma once

#include "ReflectionAPI.h"
#include <vector>
#include <cstring>

namespace detail
{
    template <typename T, typename U>
	constexpr size_t OffsetOf(U T::*member)
	{
		return (char*)&((T*)nullptr->*member) - (char*)nullptr;
	}
}

struct TypeInfo;

struct Member
{
	TypeInfo *info;
	const char *name;
	size_t offset;
};

struct TypeInfo
{
	const char *name;
	int			size;

	bool operator==(TypeInfo *other)
	{
		return name == other->name && size == other->size;
	}

	std::vector<Member> members;

	template <typename T, typename M>
	void SetMember(T *object, M T::*member, const M& newValue)
	{
		size_t offset = detail::OffsetOf(member);

		char *base = (char*)object;
		M *newPtr;

		newPtr = (M*)(base + offset);
		*newPtr = newValue;
	}

	template <typename T, typename M>
	void SetMember(T *object, const char *name, const M& newValue)
	{
		Member *member_ptr = NULL;
		for (Member& member : members)
		{
			if (std::strcmp(member.name, name) == 0)
				member_ptr = &member;
		}

		if (member_ptr == NULL)
			return;

		size_t offset = member_ptr->offset;

		char *base = (char*)object;
		M *newPtr;

		newPtr = (M*)(base + offset);
		*newPtr = newValue;
	}

};