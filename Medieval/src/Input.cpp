#include "Input.h"

#include <cmath>
#include <cstdlib>
#include <GLFW/glfw3.h>

glm::ivec2 Input::mouse_pos, Input::prev_mouse_pos;
std::set<int> Input::keys_down, Input::keys_pressed, Input::keys_released;

void Input::init()
{
    static bool once;
    if (once)
    {
        std::cout << "Input::init() called twice!\n";
        std::exit(0);
    }
    once = 1;

    glfwSetKeyCallback(Application::getInstance().getWindowHandle(),
    [](GLFWwindow *, int key, int, int action, int)
    {
        if (action == GLFW_REPEAT) // We don't care about auto key repeats.
            return;
        (action == GLFW_PRESS ? keys_pressed : keys_released).insert(key);
    });
}

void Input::tick()
{
    keys_pressed.clear();
    keys_released.clear();

    glfwPollEvents(); // <---

    // Mouse
    double tmpx, tmpy;
    glfwGetCursorPos(Application::getInstance().getWindowHandle(), &tmpx, &tmpy);
    // +.5 is here to round the values properly.
    prev_mouse_pos = mouse_pos;
    mouse_pos.x = tmpx + .5;
    mouse_pos.y = tmpy + .5;

    // Keyboard
    for (int it : keys_pressed)
        keys_down.insert(it);
    for (int it : keys_released)
    {
        auto iterator = keys_down.find(it);
        if (iterator != keys_down.end())
            keys_down.erase(iterator);
    }
}
