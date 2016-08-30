#pragma once

#include <unordered_map>
#include <functional>
#include "../introspection/IntrospectionManager.h"
#include "../introspection/Serializable.h"

#define REGISTER_COMPONENT_ID(T) \
	static ComponentRegister<T> registrar_##T;

// __COUNTER__ is not actually standard but should be alright for GCC and MSVC
#define COMPONENT(TYPE, ID) \
	static constexpr int static_id = ID; \
	TYPE() { REGISTER_TYPE(TYPE); REGISTER_MEMBER(TYPE, id); } \

#define FINISH_COMPONENT(NAME) \
	REGISTER_COMPONENT_ID(NAME)

struct Component : public Serializable
{
	virtual void Create(GameObject *object) = 0;
	virtual void Update(GameObject *object) = 0;
	virtual void Destroy() {};
	virtual ~Component() {}

	int id;
};

struct ComponentIDList
{
private:
	typedef std::unordered_map<int, std::function<Component*(void)>> map;
	map component_map;

public:
	void RegisterComponent(int id, std::function<Component*(void)> function)
	{
		map::iterator it = component_map.find(id);
		if (it != component_map.end())
			return;

		component_map[id] = function;
	}

	Component *CreateComponent(int id) {
		return component_map[id]();
	}

	static ComponentIDList *Get() { static ComponentIDList list; return &list; }
};

template<class T>
class ComponentRegister {
public:
	ComponentRegister()
	{
		T::RegisterMembers();
		// register the class factory function
		ComponentIDList::Get()->RegisterComponent(T::static_id,
			[](void) -> Component * { return new T(); });
	}
};
