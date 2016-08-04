#include "MenuExample.h"
#include "Application.h"

MenuExample::MenuExample(bool enabled) : UICanvas(enabled), 
	menuToggle(*new UIButton("res/images/ButtonNormal.png", "res/images/ButtonPressed.png", "res/images/ButtonHover.png", glm::vec2(WIDTH / 2 - 33 * 5, 0), glm::vec2(66, 25), 5.0f)),
	quitButton(*new UIButton("res/images/ButtonNormal.png", "res/images/ButtonPressed.png", "res/images/ButtonHover.png", glm::vec2(WIDTH / 2 - 33 * 5, 500), glm::vec2(66, 25), 5.0f))
{
	elements.push_back(new UIImage("res/images/hud.png", glm::vec2(), 5));
	elements.push_back(&menuToggle);
	elements.push_back(&quitButton);
}

MenuExample::~MenuExample()
{

}

void MenuExample::CanvasTick()
{
	if (menuToggle.IsPressed())
		enabled = !enabled;
	//if (quitButton.IsPressed())
		//Application::Close();
}