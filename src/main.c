#include <stdio.h>
#include <time.h>
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

#include "lib/utlis.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int hight) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0,0, width, hight);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

int main(void) {
    struct timespec start_time, end_time;

    int frame_cap = 60;
    int window_width = 800;
    int window_height = 600;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LeanOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\r\n");
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\r\n");
        return -1;
    }

    glViewport(0, 0, window_width, window_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float shape_vertices[] = {
        -0.3f, -0.3f, 0.0f,
        0.3f, -0.3f, 0.0f,
        0.0f,  0.3f, 0.0f
    };

    float vertices_shape_1[9];
    float vertices_shape_2[9];

    // shift left op
    for (int index = 0; index < 9; index ++) {
        if (index % 3 == 0) {
            vertices_shape_1[index] = shape_vertices[index] - 0.5;
        }else {
            vertices_shape_1[index] = shape_vertices[index];
        }
    }

    for (int index = 0; index < 9; index ++) {
        if (index % 3 == 0) {
            vertices_shape_2[index] = shape_vertices[index] + 0.5f;
        } else {
            vertices_shape_2[index] = shape_vertices[index];
        }
    }
    
    unsigned int indices_1[] = {  // note that we start from 0!
        0, 1, 2,
    };
    unsigned int indices_2[] = {  // note that we start from 0!
        0, 1, 2,
    };

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    int success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        return -1;
    }

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        return -1;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("ERROR::SHADER::LINKING::COMPILATION_FAILED\n%s", infoLog);
            return -1;
        }

        const char *fragmentShaderYellowSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";

    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSource, NULL);
    glCompileShader(fragmentShaderYellow);

    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::YELLOW::COMPILATION_FAILED\n%s", infoLog);
        return -1;
    }

    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShaderYellow);
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING::COMPILATION_FAILED\n%s", infoLog);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderYellow);

    // TODO: check if shader was remmoved

    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW)
    Buffer* mesh_buffers[2];
    mesh_buffers[0] = create_mesh_buffers(vertices_shape_1, sizeof(vertices_shape_1), indices_1, sizeof(indices_1), GL_STATIC_DRAW);
    mesh_buffers[1] = create_mesh_buffers(vertices_shape_2, sizeof(vertices_shape_2), indices_2, sizeof(indices_2), GL_STATIC_DRAW);
    
    {
        // unsigned int VBO, VAO, EBO;
        // // ---------------------------------------------------------------------------------
        // // set up vertex data (and buffer(s)) and configure vertex attributes
        // //   VBO = raw vertex data (positions) sitting in GPU memory
        // //   VAO = remembers HOW to read that data (attribute layout) + which EBO goes with it
        // //   EBO = index data, so the two triangles can share their 2 common corners
        // //         instead of each corner being duplicated in the VBO
        // // ---------------------------------------------------------------------------------
        // glGenVertexArrays(1, &VAO);
        // glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);

        // // 1. bind the VAO first - every buffer bind / attribute call below gets
        // //    "recorded" into this VAO until we unbind it in step 5
        // glBindVertexArray(VAO);

        // // 2. copy vertex positions into the VBO's GPU memory
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_shape_1), vertices_shape_1, GL_STATIC_DRAW);

        // // 3. copy index data into the EBO's GPU memory
        // //    (this bind is what gets stored inside the currently-bound VAO)
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // // 4. tell attribute slot 0 (aPos in the vertex shader) how to read the VBO:
        // //    3 floats per vertex, not normalized, tightly packed, starting at offset 0
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);

        // // glVertexAttribPointer already recorded the VBO into this attribute slot,
        // // so it's safe to unbind GL_ARRAY_BUFFER now - the VAO remembers it either way
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // do NOT unbind GL_ELEMENT_ARRAY_BUFFER here - that binding lives inside
        // // the VAO itself, so unbinding it now would remove the EBO from this VAO
        // // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // // 5. unbind the VAO so other glBindVertexArray/attribute calls elsewhere
        // //    in the program can't accidentally change this VAO's setup
        // glBindVertexArray(0);
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        timespec_get(&start_time, TIME_UTC);
        // input
        // -----
        process_input(window);
        
        // clear screen at the start of read render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);         

        // CAUTION:
        for (int index = 0; index < 2; index++) {
            // rendering commands here
            glUseProgram(index == 0 ? shaderProgram : shaderProgram2);
            glBindVertexArray(mesh_buffers[index]->VAO);
            glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, 0);
        }
        // re-binding the VAO is all we need - it already remembers both the
        // VBO's attribute layout AND the EBO, so there's no need to touch
        // GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER again here
        // glBindVertexArray(mesh_buffer1->VAO);
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements()

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
        timespec_get(&end_time, TIME_UTC);
        double elapsed_time_ms = (get_elapsed_seconds(start_time, end_time) * 1000.0);
        double ms_cap = 100.0 / frame_cap;

        if (elapsed_time_ms < ms_cap) {
            double duration = ms_cap / elapsed_time_ms;
            sleep_ms((int)duration);
        }
    }


    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}