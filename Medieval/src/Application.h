#pragma once
#include "includes.h"
#include "rendering/Shader.h"

#include <memory>

class Application
{
public:
	void run();

	static Application& getInstance();

private:
	GLFWwindow* window;
	
    void init();
	void mainLoop();
	void destroy();

	~Application();

};

