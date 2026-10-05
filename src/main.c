#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

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
#include "lib/shader.h"
#include "lib/stb_image.h"

#include "cglm/cglm.h"

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

    // float vertices_shape_1[] = {
    //     // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f, 0.0f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f    // top left 
    // };

    float vertices_shape_1[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // =======================  =======================

    glEnable(GL_DEPTH_TEST);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *containerTexture = stbi_load("src/assets/container.jpg", &width, &height, &nrChannels, 0);
    if (containerTexture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "ERROR::FAILED_TO_LOAD_TEXTURE;\n");
        fflush(stderr);
        exit(1);
    }
    stbi_image_free(containerTexture);

    
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *faceTexture = stbi_load("src/assets/awesomeface.png", &width, &height, &nrChannels, 0);
    if (faceTexture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        fprintf(stderr, "ERROR::FAILED_TO_LOAD_TEXTURE;\n");
        fflush(stderr);
        exit(1);
    }
    stbi_image_free(faceTexture);


    // float vertices_shape_1[9];
    // float vertices_shape_2[9];
    unsigned int indices_1[] = {  // note that we start from 0!
        0, 1, 3,  // First triangle:  Top-Right -> Bottom-Right -> Top-Left
        1, 2, 3   // Second triangle: Bottom-Right -> Bottom-Left -> Top-Left
    };

    // Shader ourShader = shader_create("../src/assets/vertex_shader.vert", "../src/assets/fragment_shader.frag");
    Shader ourShader = shader_create("src/assets/vertex_shader.vs", "src/assets/fragment_shader.fs");
  
    // TODO: check if shader was remmoved
    Buffer* mesh_buffers = create_mesh_buffers(vertices_shape_1, sizeof(vertices_shape_1), indices_1, sizeof(indices_1), GL_STATIC_DRAW);

    shader_use(&ourShader);
    
    shader_set_int(&ourShader, "texture1", 0);
    shader_set_int(&ourShader, "texture2", 1);
    
    // vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
    
    // render loop
    // -----------    
    while (!glfwWindowShouldClose(window)) {
        timespec_get(&start_time, TIME_UTC);
        // input
        // -----
        process_input(window);
        
        mat4 model, view, projection;
        glm_mat4_identity(model);
        glm_mat4_identity(view);
        glm_mat4_identity(projection);

        // 2. Apply transformations
        // Model: Rotate the quad slightly over time
        glm_rotate(model, (float)glfwGetTime() * glm_rad(50.0f), (vec3){0.5f, 1.0f, 0.0f});
        
        // View: Move the camera back 3 units so the object isn't inside our face
        glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
        
        // Projection: Use window_width and window_height (NOT width/height from the texture)
        glm_perspective(glm_rad(45.0f), (float)window_width / (float)window_height, 0.1f, 100.0f, projection);

        // 3. Send the matrices to the shader's uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);

        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);

        unsigned int projLoc  = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);

        // clear screen at the start of read render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CAUTION:
        float timeValue1 = glfwGetTime();

        glActiveTexture(GL_TEXTURE0);// activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1);// 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);// 
        
        glBindVertexArray(mesh_buffers->VAO);

        // glDrawElements(GL_TRIANGLES, mesh_buffers->index_count, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();


        timespec_get(&end_time, TIME_UTC);
        double elapsed_time_ms = (get_elapsed_seconds(start_time, end_time) * 1000.0);
        double ms_cap = 1000.0 / frame_cap;

        if (elapsed_time_ms < ms_cap) {
            double sleep_duration = ms_cap - elapsed_time_ms;
            sleep_ms((int)sleep_duration);
        }
    }


    if (mesh_buffers != NULL) {
        free(mesh_buffers);
        mesh_buffers = NULL;
    }

        // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
