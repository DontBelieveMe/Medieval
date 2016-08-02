#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::Update()
{
	for (auto& component : components)
	{
		component->update();
	}
}

void GameObject::Init()
{
	for (auto& component : components)
	{
		component->create();
	}
}