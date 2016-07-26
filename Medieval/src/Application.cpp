#include "Application.h"

#include "Shader.h"
#include "States.h"

#include "Voxels.h"
#include "Renderer2D.h"

void Application::run()
{
	init();
	mainLoop();
	destroy();
}

void Application::init()
{
	glfwInit();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE); // so we can choose when to make it visible
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // load extensions

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, videoMode->width / 2 - WIDTH / 2, videoMode->height / 2 - HEIGHT / 2);

#ifdef V_SYNC_ENABLED
	glfwSwapInterval(1);
#else
	glfwSwapInterval(0);
#endif
}

void Application::mainLoop()
{
	double lastTime = glfwGetTime(), secTime = lastTime, delta = 0, sPerTick = 1.0 / 60.0;
	int ticks = 0, frames = 0;

	glfwShowWindow(window); // The window becomes visible here

	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta += (now - lastTime) / sPerTick;
		lastTime = now;

		while (delta > 0)
		{
			glfwPollEvents();
			StateSystem::tick(); // State system is done, use it. No code here plz.
			ticks++;
			delta -= 1.0;
		}

		StateSystem::render(); // State system is done, use it. No code here plz.

		glfwSwapBuffers(window);

		frames++;

		if (glfwGetTime() - secTime >= 1.0)
		{
			cout << ticks << " ticks, " << frames << " frames" << endl;
			ticks = 0;
			frames = 0;
			secTime += 1.0;
		}
	}

	StateSystem::cleanup();
}

void Application::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

Application::~Application()
{
}

Application& Application::getInstance()
{
	static Application app;
	return app;
}