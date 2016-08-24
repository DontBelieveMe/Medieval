#pragma once

#include "ReflectionAPI.h"

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
			if (strcmp(member.name, name) == 0)
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