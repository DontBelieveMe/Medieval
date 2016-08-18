#pragma once

#include <ui/UI.h>

#include <AssetData.h>
#include <Input.h>

#include <memory>

class PauseMenu : public UICanvas
{

public:
	PauseMenu(bool enabled = true);
	~PauseMenu();

	virtual void CanvasTick();
	virtual void CanvasRender();
	
private:
	UIButton quit_button;
	UIButton resume_button;
};