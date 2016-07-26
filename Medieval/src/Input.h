#pragma once

#include <set>
#include <glm/glm.hpp>
#include "Application.h"

class Input
{
    static glm::ivec2 mouse_pos, prev_mouse_pos;
    static std::set<int> keys_down, keys_pressed, keys_released;

    friend class Application;
    static void init();
    static void tick();

  public:
    static glm::ivec2 mousePos()      {return mouse_pos;}
    static glm::ivec2 mousePosDelta() {return mouse_pos - prev_mouse_pos;}

    static bool keyDown    (int code) {return keys_down    .find(code) != keys_down    .end();}
    static bool keyPressed (int code) {return keys_pressed .find(code) != keys_pressed .end();}
    static bool keyReleased(int code) {return keys_released.find(code) != keys_released.end();}
};