#include "GameObject.h"

GameObject::GameObject()
{

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