#pragma once
#include "includes.h"
#include "rendering/Shader.h"
#include "state/State.h"

#include <memory>

class Application
{
public:
	void run();

	static Application& getInstance();
    GLFWwindow *getWindowHandle() const {return window;} 

	inline int FPS() const { return fps; }
	inline int UPS() const { return ups; }
	float getDelta() { return this->delta; }
private:
	GLFWwindow* window;

    void init();
	void mainLoop();
	void destroy();
	int fps;
	int ups;
	float delta;

	~Application();

};

