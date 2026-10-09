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

// GLFW calls this when the drawable framebuffer changes size. The viewport
// must use framebuffer pixels (not necessarily logical window points), or
// OpenGL will render into only part of a resized/high-DPI window.
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
    // The frame limiter measures each complete loop iteration. The dimensions
    // are also used for the initial viewport and projection aspect ratio.
    // Note: the resize callback updates the viewport, but these two values
    // remain unchanged, so the projection aspect ratio is fixed after startup.
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

    // Interleaved vertex format: position (x, y, z), then texture coordinates
    // (u, v). Each record is five floats; the mesh helper configures locations
    // 0 and 2 with this exact stride and offset. There are 36 vertices so each
    // cube face can use its own texture coordinates without sharing corners.
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

    // World-space translation for each cube instance. All ten cubes reuse
    // the same vertex data; the render loop supplies a different model matrix
    // for each position.
    vec3 cubPositions[10] = {
        { 0.0f,  0.0f,  0.0f },
        { 2.0f,  5.0f, -15.0f },
        {-1.5f, -2.2f, -2.5f },
        {-3.8f, -2.0f, -12.3f },
        { 2.4f, -0.4f, -3.5f },
        {-1.7f,  3.0f, -7.5f },
        { 1.3f, -2.0f, -2.5f },
        { 1.5f,  2.0f, -2.5f },
        { 1.5f,  0.2f, -1.5f },
        {-1.3f,  1.0f, -1.5f }
    };

    // Depth testing is needed because the triangles for the cubes overlap in
    // screen space. Without it, later submitted faces could cover nearer ones.
    glEnable(GL_DEPTH_TEST);

    // Texture setup follows the same sequence for each image: create and bind
    // a texture object, choose wrap/filter behavior, decode the image on the
    // CPU, upload the pixels, then generate mipmaps for minified sampling.
    // Texture unit selection is done later, immediately before drawing.
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


    // These indices are only a two-triangle example, not a full cube index
    // list. The active draw call below uses glDrawArrays for all 36 vertices,
    // so this EBO data and its index_count are currently unused.
    unsigned int indices_1[] = {  // note that we start from 0!
        0, 1, 3,  // First triangle:  Top-Right -> Bottom-Right -> Top-Left
        1, 2, 3   // Second triangle: Bottom-Right -> Bottom-Left -> Top-Left
    };

    // Shader and image paths are relative to the process working directory.
    // Run from the repository root for these src/assets/... paths to resolve.
    Shader ourShader = shader_create("src/assets/vertex_shader.vs", "src/assets/fragment_shader.fs");

    // Upload the interleaved vertex data and configure the VAO's attribute
    // layout. The EBO is created too, although this scene currently draws the
    // vertices sequentially rather than using indices.
    // TODO: check if shader was remmoved
    Buffer* mesh_buffers = create_mesh_buffers(vertices_shape_1, sizeof(vertices_shape_1), indices_1, sizeof(indices_1), GL_STATIC_DRAW);

    shader_use(&ourShader);

    // Sampler uniforms contain texture-unit numbers, not texture object IDs:
    // texture1 samples unit 0 and texture2 samples unit 1.
    shader_set_int(&ourShader, "texture1", 0);
    shader_set_int(&ourShader, "texture2", 1);

    vec3 cameraPos = {0.0f, 0.0f, 3.0f};
    vec3 targetPos = {0.0f, 0.0f, 0.0f};
    vec3 direction;

    glm_vec3_sub(cameraPos, targetPos, direction);
    glm_normalize(direction);

    // Frame procedure: handle input; update shared camera/projection uniforms;
    // clear color/depth; bind textures and VAO; draw every cube with its own
    // model matrix; present the frame and poll events; then wait toward the
    // configured cap. The sleep is approximate because milliseconds are
    // truncated and the timer uses wall-clock TIME_UTC.
    while (!glfwWindowShouldClose(window)) {
        timespec_get(&start_time, TIME_UTC);
        // input
        // -----
        process_input(window);

        mat4 model, view, projection;
        // glm_mat4_identity(model);
        glm_mat4_identity(view);
        glm_mat4_identity(projection);

        // 2. Apply transformations
        // Model: Rotate the quad slightly over time
        // glm_rotate(model, (float)glfwGetTime() * glm_rad(50.0f), (vec3){0.5f, 1.0f, 0.0f});

        // View: Move the camera back 3 units so the object isn't inside our face
        glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

        // Projection: Use window_width and window_height (NOT width/height from the texture)
        glm_perspective(glm_rad(57.0f), (float)window_width / (float)window_height, 0.1f, 100.0f, projection);

        // 3. Send the matrices to the shader's uniform locations
        // unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);

        // unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
        shader_set_mat4(&ourShader, "view", view);

        // unsigned int projLoc  = glGetUniformLocation(ourShader.ID, "projection");
        // glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);
        shader_set_mat4(&ourShader, "projection", projection);

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
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        for (unsigned int i=0; i<10;i++) {
            // Build the per-instance transform from identity. Translation
            // places the cube; the two rotations add a fixed per-cube angle
            // and a shared time-based spin. Upload it immediately before the
            // draw, since model is the only transform changing in this loop.
            glm_mat4_identity(model);

            // position models
            glm_translate(model, cubPositions[i]);

            // set initial angle
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

            // incrementally
            glm_rotate(model, (float)glfwGetTime() * glm_rad(50.0f), (vec3){0.5f, 1.0f, 0.0f});

            //  add the model vertices
            shader_set_mat4(&ourShader, "model", model);

            // draw model
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
