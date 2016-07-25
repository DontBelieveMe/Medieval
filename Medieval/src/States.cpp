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
            }
        };
    }

    /*
    StateSystem::State Template()
    {
        // Your variables here marked as static.

        // If you want to reset your variables to a some value when you enable this state, also do it here.

        return
        {
            []{ // Tick

                // Tick code here

            },

            []{ // Render

                // Render code here

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