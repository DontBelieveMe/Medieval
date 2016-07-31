#include "GameObject.h"

void GameObject::update()
{
	for (auto& component : components)
	{
		component->update();
	}
}
