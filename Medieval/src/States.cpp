#include "States.h"
#include "includes.h"

namespace States
{
    StateSystem::State EntryPoint()
    {
        static float rot;

        return
        {
            []{ // Tick
                rot+=1;
            },

            []{ // Render
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);//if something isn't showing up TRY IT WITHOUT THIS
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
			    
            },

            []{ // Cleanup

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
    State current_state = States::EntryPoint();
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
}