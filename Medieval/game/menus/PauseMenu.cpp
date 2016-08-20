#include "PauseMenu.h"

PauseMenu::PauseMenu(bool enabled)
	: UICanvas(enabled),
	quit_button(UIButton(TEXTURE_PATH("pauseMenu/exit"),
		TEXTURE_PATH("pauseMenu/exit"),
		TEXTURE_PATH("pauseMenu/exit_hover"),
		glm::vec2(WIDTH / 2 - 33 * 5, 400),
		glm::vec2(66, 25), 5.0f)),
	resume_button(UIButton(TEXTURE_PATH("pauseMenu/resume"),
		TEXTURE_PATH("pauseMenu/resume"),
		TEXTURE_PATH("pauseMenu/resume_hover"),
		glm::vec2(WIDTH / 2 - 41 * 5, 250),
		glm::vec2(66, 25), 5.0f))
{
	auto on_exit = [&]() { std::exit(0); };
	auto on_resume = [&]() { std::exit(0); };

	quit_button.SetPressedCallback(on_exit);
	resume_button.SetPressedCallback(on_resume);

	elements.push_back(&quit_button);
	elements.push_back(&resume_button);
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::CanvasTick()
{
	if (Keys::toogle_focus.Pressed())
	{
		enabled = !enabled;
		Input::SetMouseMode(Input::MouseMode::normal);
	}
}

void PauseMenu::CanvasRender()
{
	
}

/*
quit_button(UIButton(TEXTURE_PATH("pauseMenu/exit"),
TEXTURE_PATH("pauseMenu/exit"),
TEXTURE_PATH("pauseMenu/exit"),
glm::vec2(WIDTH / 2 - 33 * 5, 500),
glm::vec2(66, 25), 5.0f))
*/