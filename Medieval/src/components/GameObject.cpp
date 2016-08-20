#include "GameObject.h"

GameObject::GameObject()
{
	scale = glm::vec3(1, 1, 1);
}

void GameObject::Update()
{
	for (auto& component : components)
	{
		component->Update(this);
	}
}

void GameObject::Init()
{
	for (auto& component : components)
	{
		component->Create(this);
	}
}