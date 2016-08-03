#pragma once
#include "ui\UIRenderer.h"
#include "ui\UIObject.h"

class UIImage : public UIObject
{
public:
	UIImage(std::string imagePath, glm::vec2 topLeftUILocation, float imageScale = 1)
	{
		texture = new Texture(imagePath);
		renderer = new UIRenderer(*texture, topLeftUILocation, imageScale);
	}

	~UIImage() { delete texture; }

	void Render()
	{
		texture->Bind();
		renderer->Render();
	}

private:
	Texture *texture;
};

