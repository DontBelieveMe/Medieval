#pragma once
#include "GameObject.h"

class Entity : public GameObject
{
protected:
    //these variables may be replaced by an AABBRigidBody from the physics system
    glm::vec3 position;
    glm::vec3 dimension; // set this in the constructor of an Entity


public:

    Entity(glm::vec3 startPos);

    void tick();

    virtual void update() = 0; // this is the tick() function for the entity

};
