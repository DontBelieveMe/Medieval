#include "PhysicsComponent.h"

#include "../ObjectFactory.h"

void CollidableComponent::Update(GameObject *object)
{
	for (auto& entity_pair : ObjectFactory::Get()->GetObjectsMap())
	{
		GameObject *object = &entity_pair.second;
		if (object->HasComponent<CollidableComponent>())
		{
			// Blah, Blah, Blah
		}
	}
}