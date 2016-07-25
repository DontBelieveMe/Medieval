#pragma once

namespace StateSystem
{
    using FunctionPointer = void (*)();
    struct State
    {
        FunctionPointer tick, render, cleanup;
    };

    void setState(State (*func)());

    void tick();
    void render();
}