#pragma once
#include "includes.h"
#include "Shader.h"

#include <memory>

class Application
{
public:
	void run();

	static Application& getInstance();

private:
	GLFWwindow* window;
    std::unique_ptr<ShaderProgram> shader;
	
    void init();
	void mainLoop();
	void destroy();
	
};

