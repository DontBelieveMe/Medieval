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

	GLfloat rot = 0;

	glfwShowWindow(window); //window becomes visible here
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta += (now - lastTime) / sPerTick;
		lastTime = now;

		while (delta > 0)
		{
			rot+=1;
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(0, 1, GL_FALSE, &glm::perspective(45.0f, ((float)WIDTH / (float)HEIGHT), 0.001f, 200.0f)[0][0]);
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0, 0, -3));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(1, 1, GL_FALSE, &model[0][0]);

		glEnableVertexAttribArray(0);
		float data[] = {
			-0.5f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, data);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);

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

Application::~Application()
{
	delete shader;
}

Application& Application::getInstance()
{
	static Application app;
	return app;
}