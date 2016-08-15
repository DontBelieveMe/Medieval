#include "MenuExample.h"
#include "Application.h"

#include <Input.h>

MenuExample::MenuExample(bool enabled) : UICanvas(enabled), 
	quitButton(*new UIButton("res/images/ButtonNormal.png", "res/images/ButtonPressed.png", "res/images/ButtonHover.png", glm::vec2(WIDTH / 2 - 33 * 5, 500), glm::vec2(66, 25), 5.0f))
{
	elements.push_back(new UIImage("res/images/hud.png", glm::vec2(), 5));
	elements.push_back(&quitButton);
}

MenuExample::~MenuExample()
{

}

void MenuExample::CanvasTick()
{
	if (Keys::toogle_focus.Pressed()) {
		std::cout << std::boolalpha << enabled << std::endl;
		if (enabled)
		{
			Input::SetMouseMode(Input::MouseMode::locked);
		}
		else {
			Input::SetMouseMode(Input::MouseMode::normal);
		}
		enabled = !enabled;
	}
	if (quitButton.IsPressed())
		std::exit(0);
}