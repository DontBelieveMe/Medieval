#pragma once
#include "GameState.h"
#include "State.h"

/*
    This is my proposal for the new state system.

    The state system is very important, and this strategy allows us to have states
    in different files. It also allows us to have clear initialization, and cleanup.
    As well as a destructor for RAII encapsulation of pointers.

    The old state system is still in place, I have written this file as a proposal, and
    we will decide on which one we like best.

    @author: Ned Boulter
*/

class StateSystem
{
private:
    State* currentState;

public:

    inline void setDefaultState()
    {
        currentState = new GameState();//this will be a Menu, at some point
    }

    inline void tick()
    {
        currentState->tick();
    }

    inline void render()
    {
        currentState->render();
    }

    inline void destroy()
    {
        currentState->destroy();
    }

    inline void changeState(State* newState)
    {
        currentState->destroy();
        currentState = newState;
    }

    inline ~StateSystem()
    {
        if(currentState)
            delete currentState;
    }

    inline static StateSystem & get()
    {
        static StateSystem system;
        return system;
    }
};
