#include "Entity.h"

Entity::Entity(glm::vec3 startPos)
{
    this->position = startPos;
}

void Entity::tick()
{
    update();

    //stuff for all Entities goes here
}
