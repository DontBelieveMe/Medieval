#include "GameObject.h"

GameObject::GameObject()
{
	for (auto& component : components)
	{
		component->create();
	}
}

void GameObject::update()
{
	for (auto& component : components)
	{
		component->update();
	}
}
