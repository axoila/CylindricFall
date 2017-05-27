// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <playground/init.h>
#include <playground/tube.h>
#include <playground/ship.h>

int main( void )
{
    window = init_glfw();


    //get tube
    tube* t = new tube();
    ship* s = new ship();

    // For speed computation
    double lastTime = glfwGetTime();
    double lastFrameTime = lastTime;
    int nbFrames = 0;

    double progress = 0;
    do{

        // Measure speed
        double currentTime = glfwGetTime();
        double delta = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        progress += delta * 10;

        s->tick(window, delta);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 VPmatrix;
        mat4 foo, bar;
        make_mvp(VPmatrix, foo, bar,vec3(0, 0, progress), vec3(0, 0, progress+1));
        t->draw(VPmatrix);
        s->draw(VPmatrix, progress);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwGetKey(window, GLFW_KEY_Q ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    delete(t);
    delete(s);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

