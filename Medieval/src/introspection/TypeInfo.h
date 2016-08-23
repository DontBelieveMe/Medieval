#pragma once

struct TypeInfo;

struct Member
{
	TypeInfo *info;
	const char *name;
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
};