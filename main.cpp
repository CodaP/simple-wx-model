#define GLFW_INCLUDE_GLCOREARB
#include <iostream>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <assert.h>
#include "coastline.h"
#include "glutil.h"
#include "grib_io.h"
#include "draw_gh_program.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static GLFWwindow* setup_glfw(){
    if (!glfwInit()) {
        // Initialization failed
        std::cerr << "Init Failed" << std::endl;
        exit(1);
    }
    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Simple Wx Model", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        std::cerr << "Window failed" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    return window;
}

int main(void) {
    read_grib_file(gh, sizeof(gh));
    GLFWwindow* window = setup_glfw();

    /* Clear screen */
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    /* Init GH program */
    init_gh();

    /* Init coast program */
    init_coast();

    /* Draw GH */
    draw_gh();
    /* Draw coast */
    draw_coast();

    glfwSwapBuffers(window); // Need to swap buffers to show

    while (!glfwWindowShouldClose(window))
    {
        // Keep running
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
