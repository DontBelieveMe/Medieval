#pragma once

class State
{
public:
    virtual void tick() = 0;

    virtual void render() = 0;

    virtual void destroy() = 0;

    virtual ~State() {}
};

