#include "States.h"
#include "Voxels.h"
#include "includes.h"
#include "application.h"

#include "Renderer2D.h"

namespace States
{
    StateSystem::State entryPoint()
    {
		static ShaderProgram modelShader("res/vert.shader", "res/frag.shader");
		static ShaderProgram uiShader("res/vert2D.shader", "res/frag2D.shader");

        static Voxels vox(4);
        static Model ent     = vox.loadModel("res/models/Ent.obj",     "res/models/Ent.png");
        static Model door    = vox.loadModel("res/models/Door.obj",    "res/models/Door.png");
        static Model ruu     = vox.loadModel("res/models/Ruu.obj",     "res/models/Ruu.png");
        static Model grothar = vox.loadModel("res/models/Grothar.obj", "res/models/Grothar.png");
        vox.setDrawingStage();

        static GLfloat rot = 0;
		
		modelShader.use();
		modelShader.uploadMatrix4f(0, glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.001f, 10000.0f));
		modelShader.halt();

		float vertices[] = {
			-1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			0.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f
		};

		//float quadvertices[] = {
		//	-0.5f, 0.5f, 0.f, 1.0f, 0.0f, 0.0f,
		//	-0.5f, -0.5f, 0.f, 0.0f, 1.0f, 0.0f,
		//	0.5f, -0.5f, 0.f, 0.0f, 0.0f, 1.0f,
		//	0.5f, -0.5f, 0.f, 1.0f, 0.0f, 0.0f,
		//	0.5f, 0.5f, 0.f, 0.0f, 1.0f, 0.0f,
		//	-0.5f, 0.5f, 0.f, 0.0f, 1.0f, 0.0f
		//};

		static Renderer2D r2d;
		r2d.createVertexArray(vertices, sizeof(vertices));

		glEnable(GL_DEPTH_TEST);
        return
        {
            []{ // Tick
                rot += 1;
            },

            []{ // Render
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_CULL_FACE);
				modelShader.use();
                vox.bind();
				modelShader.uploadMatrix4f(1, glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(-6, -2, -15)), glm::radians(rot), glm::vec3(0, 1, 0)));
                drawModel(ruu);
				modelShader.uploadMatrix4f(1, glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(6, -2, -15)), glm::radians(rot), glm::vec3(0, 1, 0)));
                drawModel(grothar);
				
				// To disable the temporary 2D rendering on the screen
				// Comment out these 5 lines
				modelShader.halt();
				glDisable(GL_CULL_FACE);
				uiShader.use();
				r2d.draw();
				uiShader.halt();
				// To here :P
			},

            []{ // Cleanup
                vox.halt();
                vox.destroy();
				r2d.destroy();
            }
        };
    }

    /*
    StateSystem::State Template()
    {
        // Put your variables here marked as static.

        // Do your init here.

        return
        {
            []{ // Tick

                // Tick code here

            },

            []{ // Render

                // Render code here

            },

            []{ // Cleanup

                // This will be called when you leave the state

            }
        };
    }
    */
}

namespace StateSystem
{
    static State boot()
    {
        return
        {
            []{setState(States::entryPoint);},
            []{},
            []{},
        };
    }

    State current_state = boot();
    State (*next_state)();

    void setState(State (*func)())
    {
        next_state = func;
    }

    void tick()
    {
        current_state.tick();
        while (next_state)
        {
            current_state.cleanup();
            current_state = next_state();
            next_state = 0;
            current_state.tick();
        }
    }
    void render()
    {
        current_state.render();
    }
    void cleanup()
    {
        current_state.cleanup();
        current_state = boot();
    }
}