#include "States.h"
#include "Voxels.h"
#include "includes.h"
#include "application.h"

namespace States
{
    StateSystem::State entryPoint()
    {
        static Voxels vox(4);
        static Model ent     = vox.loadModel("res/models/Ent.obj",     "res/models/Ent.png");
        static Model door    = vox.loadModel("res/models/Door.obj",    "res/models/Door.png");
        static Model ruu     = vox.loadModel("res/models/Ruu.obj",     "res/models/Ruu.png");
        static Model grothar = vox.loadModel("res/models/Grothar.obj", "res/models/Grothar.png");
        vox.setDrawingStage();

        static GLfloat rot = 0;

        Application::getInstance().Shader().uploadMatrix4f(0, glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.001f, 10000.0f));

        return
        {
            []{ // Tick
                rot += 1;
            },

            []{ // Render
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                vox.bind();
                Application::getInstance().Shader().uploadMatrix4f(1, glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(-6, -2, -15)), glm::radians(rot), glm::vec3(0, 1, 0)));
                drawModel(ruu);
                Application::getInstance().Shader().uploadMatrix4f(1, glm::rotate(glm::translate(glm::mat4(1.0), glm::vec3(6, -2, -15)), glm::radians(rot), glm::vec3(0, 1, 0)));
                drawModel(grothar);
            },

            []{ // Cleanup
                vox.halt();
                vox.destroy();
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