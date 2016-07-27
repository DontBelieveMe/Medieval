#include "Input.h"

#include "Utils.h"

#include <cmath>
#include <cstdlib>
#include <GLFW/glfw3.h>

glm::ivec2 Input::mouse_pos, Input::prev_mouse_pos;
std::set<int> Input::keys_down, Input::keys_pressed, Input::keys_released;
bool Input::mouse_down[max_mouse_buttons], Input::mouse_pressed[max_mouse_buttons], Input::mouse_released[max_mouse_buttons];
glm::dvec2 Input::scroll;

void Input::init()
{
    static bool once;
    if (once)
        Error("Input::init() was called twice!");
    once = 1;

    glfwSetKeyCallback(Application::getInstance().getWindowHandle(),
    [](GLFWwindow *, int key, int, int action, int)
    {
        if (action == GLFW_REPEAT) // We don't care about auto key repeats.
            return;
        (action == GLFW_PRESS ? keys_pressed : keys_released).insert(key);
    });

    glfwSetMouseButtonCallback(Application::getInstance().getWindowHandle(),
    [](GLFWwindow *, int button, int action, int)
    {
        // GLFW_REPEAT can't occur here.
        if (button >= 8)
            return;
        (action == GLFW_PRESS ? mouse_pressed : mouse_released)[button] = 1;
    });

    glfwSetScrollCallback(Application::getInstance().getWindowHandle(),
    [](GLFWwindow *, double x, double y)
    {
        scroll.x += x;
        scroll.y += y;
    });
}

void Input::tick()
{
    // Clear mouse buttons
    for (int i = 0; i < max_mouse_buttons; i++)
        mouse_pressed[i] = mouse_released[i] = 0;

    // Clear keyboard keys
    keys_pressed.clear();
    keys_released.clear();

    // Clear scroll
    scroll.x = scroll.y = 0;

    glfwPollEvents(); // <---

    // Mouse position
    double tmpx, tmpy;
    glfwGetCursorPos(Application::getInstance().getWindowHandle(), &tmpx, &tmpy);
    // +.5 is here to round the values properly.
    prev_mouse_pos = mouse_pos;
    mouse_pos.x = tmpx + .5;
    mouse_pos.y = tmpy + .5;

    // Mouse buttons
    for (int i = 0; i < max_mouse_buttons; i++)
    {
        if (mouse_pressed[i])
            mouse_down[i] = 1;
        if (mouse_released[i])
            mouse_down[i] = 0;
    }
    // Keyboard keys
    for (int it : keys_pressed)
        keys_down.insert(it);
    for (int it : keys_released)
    {
        auto iterator = keys_down.find(it);
        if (iterator != keys_down.end())
            keys_down.erase(iterator);
    }
}
