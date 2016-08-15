#pragma once
#include "includes.h"
#include "rendering/Shader.h"
#include "state/State.h"

#include <memory>

class Application
{
public:
	void Run();

	static Application& GetInstance();
    GLFWwindow *GetWindowHandle() const {return window;} 

	inline int FPS() const { return fps; }
	inline int UPS() const { return ups; }
	float GetDelta() { return this->delta; }

	static glm::mat4 projection_matrix;

private:
	GLFWwindow* window;

    void Init();
	void MainLoop();
	void Destroy();
	int fps;
	int ups;
	float delta;

	~Application();

};

