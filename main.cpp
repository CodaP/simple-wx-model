#include <iostream>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    if (!glfwInit()) {
        // Initialization failed
        std::cerr << "Init Failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(glfw_error_callback);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Wx Model", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        std::cerr << "Window failed" << std::endl;
        return 1;
    }
    glfwMakeContextCurrent(window);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window))
    {
        // Keep running
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
