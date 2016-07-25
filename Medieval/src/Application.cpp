#include "Application.h"

#include "Shader.h"

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

    shader = new ShaderProgram("res/vert.shader", "res/frag.shader");
    shader->use();
}

void Application::mainLoop()
{
	
	double lastTime = glfwGetTime(), secTime = glfwGetTime(), delta = 0, sPerTick = 1.0 / 60.0;
	int ticks = 0, frames = 0;

	glfwShowWindow(window); //window becomes visible here
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta += (now - lastTime) / sPerTick;
		lastTime = now;

		while (delta > 0)
		{
			ticks++;
			glfwPollEvents();
			delta -= 1.0;
		}


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
	glfwDestroyWindow(window);
	glfwTerminate();
}

Application& Application::getInstance()
{
	static Application app;
	return app;
}