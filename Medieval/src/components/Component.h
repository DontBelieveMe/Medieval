#pragma once

// __COUNTER__ is not actually standard but should be alright for GCC and MSVC
#define COMPONENT static constexpr int static_id = __COUNTER__;

struct Component
{
	
	virtual void Create() = 0;
	virtual void Update(GameObject *object) = 0;

	int id;
};