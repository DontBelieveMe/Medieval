#include "Application.h"

#include "rendering/Shader.h"
//#include "state/States.h"
#include "rendering/AdditionalVAO.h"
#include "state/StateSystem.h"
#include "Input.h"

#include "rendering/Voxels.h"
#include "rendering/Renderer2D.h"

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

    StateSystem::get().setDefaultState();
    Input::init();

    AdditionalVAO::init();//same for all states, so initialized here, needs to be bound on use.
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
		    Input::tick(); // PollEvents(); has been moved here.
            StateSystem::get().tick();
			ticks++;
			delta -= 1.0;
		}

        StateSystem::get().render();

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
}

void Application::destroy()
{
    StateSystem::get().destroy();
    AdditionalVAO::destroy();//states don't need to worry about this
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