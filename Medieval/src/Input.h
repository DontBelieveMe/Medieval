#pragma once

#include <set>
#include <glm/glm.hpp>
#include "Application.h"

class Input
{
  public:
    static constexpr int max_mouse_buttons = 8;

  private:
    static glm::ivec2 mouse_pos, prev_mouse_pos;
    static std::set<int> keys_down, keys_pressed, keys_released;
    static bool mouse_down[max_mouse_buttons], mouse_pressed[max_mouse_buttons], mouse_released[max_mouse_buttons];
    static glm::dvec2 scroll;

    friend class Application;
    static void init();
    static void tick();

  public:
    static glm::ivec2 mousePos()      {return mouse_pos;}
    static glm::ivec2 mousePosDelta() {return mouse_pos - prev_mouse_pos;}

    /* < Key and button functions noob guide >
     * When you press a key following happens:
     *              ----------time-------->
     * Finger is on button:         #######
     *               Down() ... 0 0 1 1 1 1 0 0 ...
     *            Pressed() ... 0 0 1 0 0 0 0 0 ...
     *           Released() ... 0 0 0 0 0 0 1 0 ...
     */

    // Left = 1, Right = 2, Middle = 3.
    static bool mouseButtonDown    (int code) {return (code != 0 && code <= max_mouse_buttons) && mouse_down    [code-1];}
    static bool mouseButtonPressed (int code) {return (code != 0 && code <= max_mouse_buttons) && mouse_pressed [code-1];}
    static bool mouseButtonReleased(int code) {return (code != 0 && code <= max_mouse_buttons) && mouse_released[code-1];}
    // Use GLFW_KEY_* enum.
    static bool keyDown    (int code) {return keys_down    .find(code) != keys_down    .end();}
    static bool keyPressed (int code) {return keys_pressed .find(code) != keys_pressed .end();}
    static bool keyReleased(int code) {return keys_released.find(code) != keys_released.end();}

    static bool wheelUp   () {return scroll.y > 0;}
    static bool wheelDown () {return scroll.y < 0;}
    static bool wheelLeft () {return scroll.x < 0;}
    static bool wheelRight() {return scroll.x > 0;}
};