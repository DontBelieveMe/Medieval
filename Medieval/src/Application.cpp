#include "Application.h"

#include "Shader.h"
#include "States.h"

#include "Voxels.h"

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

	shader = new ShaderProgram("res/vert.shader", "res/frag.shader");
	shader->use();
}

void Application::mainLoop()
{
	double lastTime = glfwGetTime(), secTime = lastTime, delta = 0, sPerTick = 1.0 / 60.0;
	int ticks = 0, frames = 0;

    Voxels vox(2);
    Model ent = vox.loadModel("res/models/Ent.obj", "res/models/Ent.png");
    Model door = vox.loadModel("res/models/Door.obj", "res/models/Door.png");
    vox.setDrawingStage();
    vox.bind();

    GLfloat rot = 0;

	glfwShowWindow(window); //window becomes visible here
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta += (now - lastTime) / sPerTick;
		lastTime = now;

		while (delta > 0)
		{
			glfwPollEvents();
            rot += 1.0;
			//StateSystem::tick();
			ticks++;
			delta -= 1.0;
		}

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->uploadMatrix4f(0, glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.001f, 10000.0f));

        srand(894759348754l);
        for (int i = 0; i < 1000; i++)
        {
            shader->uploadMatrix4f(1, glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3((rand() % 400) - 100, (rand() % 400) - 100, -60 - rand() % 400)), glm::radians(rot), glm::vec3(0, 1, 0)));
            if (rand() % 2 == 0)
                drawModel(ent);
            else
                drawModel(door);
        }

       // StateSystem::render();

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

    vox.halt();
    vox.destroy();
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