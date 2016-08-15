#include "Input.h"

#include "Utils.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <map>
#include <string>
#include <GLFW/glfw3.h>

static constexpr const char *controls_config_file_name = "controls.config";

glm::ivec2 Input::mouse_pos, Input::prev_mouse_pos;
std::set<int> Input::keys_down, Input::keys_pressed, Input::keys_released;
bool Input::mouse_down[max_mouse_buttons], Input::mouse_pressed[max_mouse_buttons], Input::mouse_released[max_mouse_buttons];
glm::dvec2 Input::scroll;

Input::MouseMode mmode;

#define CONFIGURABLE(token, value) {token, value},
#define STATIC(token, value) /* nothing */
#define KEY(token, name, mode, value) mode(#token, value)
static std::map<std::string, int> config{INPUT_KEYS_LIST};
#undef CONFIGURABLE
#undef STATIC
#undef KEY

#define KEY(token, name, mode, value) {#token, &Keys::token},
static const std::map<std::string, Input::Key *> key_addresses{INPUT_KEYS_LIST};
#undef KEY

void Input::Init()
{
    static bool once;
    if (once)
        Error("Input::init() was called twice!");
    once = 1;

    glfwSetKeyCallback(Application::GetInstance().GetWindowHandle(),
    [](GLFWwindow *, int key, int, int action, int)
    {
        if (action == GLFW_REPEAT) // We don't care about auto key repeats.
            return;
        (action == GLFW_PRESS ? keys_pressed : keys_released).insert(key);
    });

    glfwSetMouseButtonCallback(Application::GetInstance().GetWindowHandle(),
    [](GLFWwindow *, int button, int action, int)
    {
        // GLFW_REPEAT can't occur here.
        if (button >= 8)
            return;
        (action == GLFW_PRESS ? mouse_pressed : mouse_released)[button] = 1;
    });

    glfwSetScrollCallback(Application::GetInstance().GetWindowHandle(),
    [](GLFWwindow *, double x, double y)
    {
        scroll.x += x;
        scroll.y += y;
    });


    double x, y;
    glfwGetCursorPos(Application::GetInstance().GetWindowHandle(), &x, &y);
    mouse_pos = prev_mouse_pos = {x,y};


    // Reading controls from file
    std::ifstream config_file(controls_config_file_name);
    if (config_file)
    {
        while (1)
        {
            std::string a;
            int b;
            config_file >> a >> b;
            if (!config_file)
                break;
            auto it = config.find(a);
            auto ptr_it = key_addresses.find(a);
            if (it == config.end() || ptr_it == key_addresses.end())
                continue;
            it->second = b;
            ptr_it->second->code = b;
        }
        config_file.close();
    }
}

void Input::Tick()
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
    glfwGetCursorPos(Application::GetInstance().GetWindowHandle(), &tmpx, &tmpy);
    // +.5 is here to round the values properly.
    prev_mouse_pos = mouse_pos;
    mouse_pos.x = int(tmpx + .5);
    mouse_pos.y = int(tmpy + .5);

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

int Input::AnyMouseButtonDown()
{
    for (int i = 0; i < max_mouse_buttons; i++)
        if (mouse_down[i])
            return i + 1;
    return 0;
}
int Input::AnyMouseButtonPressed()
{
    for (int i = 0; i < max_mouse_buttons; i++)
        if (mouse_pressed[i])
            return i + 1;
    return 0;
}
int Input::AnyMouseButtonReleased()
{
    for (int i = 0; i < max_mouse_buttons; i++)
        if (mouse_released[i])
            return i + 1;
    return 0;
}

int Input::AnyKeyDown()
{
    if (keys_down.size() == 0)
        return 0;
    return *keys_down.begin();
}
int Input::AnyKeyPressed()
{
    if (keys_pressed.size() == 0)
        return 0;
    return *keys_pressed.begin();
}
int Input::AnyKeyReleased()
{
    if (keys_released.size() == 0)
        return 0;
    return *keys_released.begin();
}

void Input::SetMouseMode(MouseMode mode)
{
    mmode = mode;
    glfwSetInputMode(Application::GetInstance().GetWindowHandle(), GLFW_CURSOR, (int)mode);
}

Input::MouseMode Input::GetMouseMode()
{
    return mmode;
}

void Input::Key::Set(int value)
{
    if (!configurable)
        Error("Attempt to change non-configurable key \"", name, "\".");

    code = value;

    config[internal_name] = value;

    std::ofstream config_file(controls_config_file_name);
    if (config_file)
    {
        for (const auto &it : config)
        {
            config_file << it.first << ' ' << it.second << '\n';
        }
        config_file.close();
    }
}

namespace Keys
{
    #define CONFIGURABLE  1
    #define STATIC        0
    #define KEY(token, name, mode, value) Input::Key token(#token, name, mode, value);
    INPUT_KEYS_LIST
    #undef CONFIGURABLE
    #undef STATIC
    #undef KEY

    int keyCount()
    {
        #define KEY(token, name, mode, value) 0,
        return (int)std::initializer_list<int>{INPUT_KEYS_LIST}.size();
        #undef KEY
    }

    Input::Key &getKey(int pos)
    {
        #define KEY(token, name, mode, value) if (pos == 0) return token; pos--;
        INPUT_KEYS_LIST
        #undef KEY
        Error("Wrong key id: ", pos);
    }

    int configurableKeyCount()
    {
        #define CONFIGURABLE 0,
        #define STATIC /* nothing */
        #define KEY(token, name, mode, value) mode
        return (int)std::initializer_list<int>{INPUT_KEYS_LIST}.size();
        #undef CONFIGURABLE
        #undef STATIC
        #undef KEY
    }
    Input::Key &getConfigurableKey(int pos)
    {
        #define CONFIGURABLE(token) if (pos == 0) return token; pos--;
        #define STATIC(token) /* nothing */
        #define KEY(token, name, mode, value) mode(token)
        INPUT_KEYS_LIST
        #undef CONFIGURABLE
        #undef STATIC
        #undef KEY
        Error("Wrong configurable key id: ", pos);
    }
}
