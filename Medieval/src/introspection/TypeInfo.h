#pragma once

struct TypeInfo
{
	const char *name;
	int			size;

	bool operator==(TypeInfo *other) 
	{
		return name == other->name && size == other->size;
	}
};