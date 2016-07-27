#pragma once
#include "../includes.h"

enum ObjectType
{
    PLAYER, ENT, TREE, THING, EXPLOSION, CHEESE, RABBIT, NPCVILLAGER, DRAGON, FIRELORD3000, BOB,//i was just typing stuff, you can remove these :P
};

class GameObject
{
public:

    virtual void tick() = 0;

    virtual void render() = 0;

    virtual ObjectType getType() = 0; //as instance testing is a pain in c++

};
