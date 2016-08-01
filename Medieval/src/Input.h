// Input system by Egor

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

    // These return a button number or 0 if there is none.
    static int mouseButtonAnyDown();
    static int mouseButtonAnyPressed();
    static int mouseButtonAnyReleased();

    // Use GLFW_KEY_* enum.
    static bool keyDown    (int code) {return keys_down    .find(code) != keys_down    .end();}
    static bool keyPressed (int code) {return keys_pressed .find(code) != keys_pressed .end();}
    static bool keyReleased(int code) {return keys_released.find(code) != keys_released.end();}

    // These return a button number or 0 if there is none.
    static int keyAnyDown();
    static int keyAnyPressed();
    static int keyAnyReleased();

    static bool wheelUp   () {return scroll.y > 0;}
    static bool wheelDown () {return scroll.y < 0;}
    static bool wheelLeft () {return scroll.x < 0;}
    static bool wheelRight() {return scroll.x > 0;}

    // `hidden` just hides mouse. Sseful if you want to draw your own cursor.
    // `locked` hides mouse and prevents it from leaving the window. Useful for 3D camera control.
    enum class MouseMode
    {
        normal = GLFW_CURSOR_NORMAL,
        hidden = GLFW_CURSOR_HIDDEN,
        locked = GLFW_CURSOR_DISABLED,
    };
    static void setMouseMode(MouseMode mode);


    class Key
    {
        // This class is for internal use. Do not use it directly. Use namespace Keys below;

        friend class Input;

        int internal_id, code;
        std::string internal_name, name;
        bool configurable;

      public:
        Key(const char *key_name_internal, const char *key_name, bool can_change, int value)
        {
            static int counter = 0;
            internal_id = counter++;
            code = value;
            internal_name = key_name_internal;
            name = key_name;
            configurable = can_change;
        }

        int getCode() const
        {
            return code;
        }

        bool isConfigurable() const
        {
            return configurable;
        }

        std::string getName() const
        {
            return name;
        }

        std::string getInternalName() const
        {
            return internal_name;
        }

        // This changes the value and updates the config file.
        void set(int value);

        bool down() const
        {
            return keyDown(code);
        }
        bool pressed() const
        {
            return keyPressed(code);
        }
        bool released() const
        {
            return keyReleased(code);
        }
    };
};

namespace Keys
{
    /* < How to register a new key - noob guide. >
     * Add following below:
     *     KEY(name, "Full name", mode, value) \
     * `name`        - The name you want to use in the code. It is also used in the config file.
     * `"Full name"` - The name you want to display.
     * `mode`        - Can be CONFIGURABLE or STATIC. The first is saved to config and can be changed, the second is not.
     * `value`       - Default value. Use GLFW_KEY_* enum.
     *
     * After that Keys::name will represent your new key. See class Input::Key for interface.
     */
    #define INPUT_KEYS_LIST \
        KEY(toggle_ui,    "Toggle UI", CONFIGURABLE, GLFW_KEY_U) \
		KEY(toggle_audio, "Toggle Audio", CONFIGURABLE, GLFW_KEY_P) \
		KEY(forward, "Forward", CONFIGURABLE, GLFW_KEY_W) \
		KEY(back,    "Back",    CONFIGURABLE, GLFW_KEY_S) \
		KEY(left,    "Left",    CONFIGURABLE, GLFW_KEY_A) \
		KEY(right,   "Right",   CONFIGURABLE, GLFW_KEY_D) \
		KEY(up,      "Up",      CONFIGURABLE, GLFW_KEY_SPACE) \
		KEY(down,    "Down",    CONFIGURABLE, GLFW_KEY_LEFT_SHIFT) \

    // Contains Key objects listed above. Tokens are used as names.
    #define KEY(token, name, mode, value) extern Input::Key token;
    INPUT_KEYS_LIST
    #undef KEY

    // Amount of keys.
    int keyCount();
    // Returns a reference to a specific key. Use Key::name instead if possible. Useful for "for" loops.
    Input::Key &getKey(int pos);

    // Same for configurable keys.
    int configurableKeyCount();
    Input::Key &getConfigurableKey(int pos);
}