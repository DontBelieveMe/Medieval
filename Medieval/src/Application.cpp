#include "Application.h"

#include "rendering/Shader.h"
#include "rendering/AdditionalVAO.h"
#include "state/StateSystem.h"
#include "Input.h"

#include "rendering/Voxels.h"
#include "rendering/Renderer2D.h"

glm::mat4 Application::projection_matrix = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.005f, 200.0f);

void Application::Run()
{
	Init();
	MainLoop();
	Destroy();
}

void Application::Init()
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
    Input::Init();

    AdditionalVAO::init();//same for all states, so initialized here, needs to be bound on use.
}

void Application::MainLoop()
{	
	double lastTime = glfwGetTime(), secTime = lastTime, sPerTick = 1.0 / 60.0;
	int ticks = 0, frames = 0;
	delta = 0;
	glfwShowWindow(window); // The window becomes visible here

	(void)ticks; // ticks is unused. This silences the warning.

	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta += float((now - lastTime) / sPerTick);
		lastTime = now;

		while (delta > 0)
		{
		    Input::Tick(); // PollEvents(); has been moved here.
            StateSystem::get().tick();
			ups++;
			delta -= 1.0;
		}

        StateSystem::get().render();

        glfwSwapBuffers(window);
		frames++;

		if (glfwGetTime() - secTime >= 1.0)
		{
			fps = frames;
			ups = 0;
			frames = 0;
			secTime += 1.0;
		}
	}
}

void Application::Destroy()
{
    StateSystem::get().destroy();
    AdditionalVAO::destroy();//states don't need to worry about this
	glfwDestroyWindow(window);
	glfwTerminate();
}

Application::~Application()
{
}

Application& Application::GetInstance()
{
	static Application app;
	return app;
}