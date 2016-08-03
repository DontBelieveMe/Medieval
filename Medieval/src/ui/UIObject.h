#pragma once
#include "../src/ui/UIRenderer.h"
#include "../src/rendering/Texture.h"

class UIObject
{
public:
	virtual ~UIObject() { if(renderer != nullptr) delete renderer; }

	virtual void Render() = 0;
	virtual void Tick() {};

	UIRenderer &GetRenderer() const { return *renderer; }

protected:
	UIRenderer *renderer;
};