#pragma once

#include <functional>

#include "../src/ui/UIRenderer.h"
#include "../src/rendering/Texture.h"
#include "../src/ui/UIObject.h"
#include "Utils.h"

class UIButton : public UIObject
{
public:
	UIButton(const std::string &buttonNormalTexture, const std::string &buttonPressedTexture, const std::string &buttonHoverTexture, const glm::vec2 &topLeftPos, const glm::vec2 &size, const float &imageScale = 1.0f); //Size = Width/Height
	~UIButton();

	void Tick();
	void Render();

	bool IsMouseOver() const { return _state == HOVER; }

	int GetState() { return _state; }
	
	void SetPressedCallback(const std::function<void(void)>& clbk) { on_pressed = clbk; }

	enum { NORMAL, HOVER, NUM_BUTTON_STATES };
private:
	Bounds2D *bounds;

	unsigned int _state;

	Texture *textures[NUM_BUTTON_STATES];
	std::function<void(void)> on_pressed;

};

