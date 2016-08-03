#include "UIButton.h"
#include "Input.h"

UIButton::UIButton(const std::string &buttonNormalTexture, const std::string &buttonPressedTexture, const std::string &buttonHoverTexture, const glm::vec2 &topLeftPos, const glm::vec2 &size, const float &imageScale)
{
	textures[NORMAL] = new Texture(buttonNormalTexture);
	textures[PRESSED] = new Texture(buttonPressedTexture);
	textures[HOVER] = new Texture(buttonHoverTexture);
	renderer = new UIRenderer(*textures[NORMAL], topLeftPos, imageScale);

	bounds = new Bounds2D(topLeftPos, glm::vec2((topLeftPos.x + size.x) * imageScale, (topLeftPos.y + size.y) * imageScale));
	_state = NORMAL;
}

UIButton::~UIButton()
{
	for (int i = 0; i < NUM_BUTTON_STATES; i++)
		delete textures[i];
	delete bounds;
}

void UIButton::Tick()
{
	glm::vec2 mousePos = Input::MousePos();
	if (bounds->Contains(mousePos))
	{
		if (Input::MouseButtonDown(1))
			_state = PRESSED;
		else
			_state = HOVER;
	}
	else
		_state = NORMAL;
}

void UIButton::Render()
{
	textures[_state]->Bind();
	renderer->Render();
}