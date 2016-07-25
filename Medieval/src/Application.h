#pragma once
#include "includes.h"
#include "Shader.h"

#include <memory>

class Application
{
public:
	void run();

	static Application& getInstance();

    ShaderProgram &Shader() {return *shader;}

private:
	GLFWwindow* window;
	ShaderProgram* shader;

    void init();
	void mainLoop();
	void destroy();

	~Application();

};

