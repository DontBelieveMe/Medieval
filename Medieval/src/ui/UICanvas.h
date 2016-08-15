#pragma once
#include "ui\UIObject.h"

/*I'd recommend against simply putting buttons in the element array as it renders them basically useless.
  take a pointer reference TO the button in the inherited class first.
*/

class UICanvas
{
public:
	bool enabled;
	UICanvas(bool enabled = true) { this->enabled = enabled; }
	virtual ~UICanvas() { for (unsigned int i = 0; i < elements.size(); i++) delete elements.at(i); }

	void Tick() 
	{
		// Hmmmm, Look into this. Moved this line for a temporary fix
		// Barney
		CanvasTick(); 
		
		if (!enabled)
		{
			return;
		}
		for (unsigned int i = 0; i < elements.size(); i++) 
			elements.at(i)->Tick(); 
	}
	void Render() 
	{
		if (!enabled)
		{
			return;
		}
		for (unsigned int i = 0; i < elements.size(); i++) 
			elements.at(i)->Render(); CanvasRender();
	}
	
protected:
	std::vector <UIObject*> elements;
	virtual void CanvasTick() {}
	virtual void CanvasRender() {}
};