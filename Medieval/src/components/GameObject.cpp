#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::update()
{
	for (auto& component : components)
	{
		component->update();
	}
}

void GameObject::init()
{
	for (auto& component : components)
	{
		component->create();
	}
}