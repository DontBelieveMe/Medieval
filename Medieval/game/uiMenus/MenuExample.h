#pragma once
#include "../src/ui/UI.h"

class MenuExample : public UICanvas
{
public:
	MenuExample(bool enabled = true);
	~MenuExample();

	void CanvasTick();

private:
	UIButton quitButton;
};

