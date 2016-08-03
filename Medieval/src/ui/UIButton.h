#pragma once
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

	bool IsPressed() const { return _state == PRESSED; }
	bool IsMouseOver() const { return _state == HOVER || _state == PRESSED; }

private:
	Bounds2D *bounds;

	unsigned int _state;
	enum { PRESSED, NORMAL, HOVER, NUM_BUTTON_STATES };

	Texture *textures[NUM_BUTTON_STATES];
};

