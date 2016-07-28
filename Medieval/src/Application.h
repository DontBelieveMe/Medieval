#pragma once
#include "includes.h"
#include "rendering/Shader.h"

#include <memory>

class Application
{
public:
	void run();

	static Application& getInstance();
    GLFWwindow *getWindowHandle() const {return window;} 

	inline int FPS() const { return fps; }
	inline int UPS() const { return ups; }

private:
	GLFWwindow* window;

    void init();
	void mainLoop();
	void destroy();
	int fps;
	int ups;

	~Application();

};

