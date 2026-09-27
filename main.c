#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32)
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms)
#else
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
    #define sleep_ms(ms) nanosleep(&(struct timespec){ \
        .tv_sec = (ms) / 1000, \
        .tv_nsec = ((ms) % 1000) * 1000000L \
    }, NULL)
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main(void) {
    int window_width = 800;
    int window_height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LeanOpenGL", NULL, NULL);

    if (window == NULL) {
        printf("Failed to create GLFW window\r\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\r\n");
        return -1;
    }

    glViewport(0, 0, window_width, window_height);


    while (1) {
        printf("loop active cycle started\r\n");
        sleep_ms(1000);
    }

    printf("Hello, OpenGl\r\n");
    return 0;
}