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

    shader = std::make_unique<ShaderProgram>("res/vert.shader", "res/frag.shader");
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
        
        float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
        };

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLuint pos;
        glGenBuffers(1, &pos);
        glBindBuffer(GL_ARRAY_BUFFER, pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        shader->use();
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pos);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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