#include "utlis.h"
#include <glad/glad.h>
#include <stdlib.h>
#include "cglm/cglm.h"

double get_elapsed_seconds(struct timespec start, struct timespec end) {
    // timespec stores whole seconds and a nanosecond remainder. Convert both
    // endpoints to the same unit before subtracting; callers can multiply the
    // result by 1000 when they need milliseconds.
    double start_sec = (double)start.tv_sec
                   + ((double)start.tv_nsec / 1000000000.0);

    double end_sec = (double)end.tv_sec
                   + ((double)end.tv_nsec / 1000000000.0);

    return end_sec - start_sec;
}

// void load_model(mat4 model);

void camera_reposition(vec3 targetPosition, vec3 cameraPos) {
    vec3 direction;

    glm_vec3_sub(cameraPos, targetPosition, direction);
    glm_normalize(direction);
}


// Upload vertex/index bytes and capture the vertex-input configuration in a
// VAO. The current layout is deliberately fixed: location 0 is vec3 position,
// location 1 is a constant white color, and location 2 is vec2 UV, all using
// a five-float interleaved record. The caller must provide a current OpenGL
// context and arrays/sizes matching this layout.
Buffer* create_mesh_buffers(float vertices[], size_t vertices_size, int indices[], size_t indices_size, GLenum usage) {
    Buffer* buf = malloc(sizeof(Buffer));

    // This is an element count, not a byte count. The current main.c draw
    // path uses glDrawArrays, so it does not consume this value.
    buf->index_count = indices_size / sizeof(unsigned int);

    // ---------------------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    //   VBO = raw vertex data (positions) sitting in GPU memory
    //   VAO = remembers HOW to read that data (attribute layout) + which EBO goes with it
    //   EBO = index data, so the two triangles can share their 2 common corners
    //         instead of each corner being duplicated in the VBO
    // ---------------------------------------------------------------------------------
    glGenVertexArrays(1, &buf->VAO);
    glGenBuffers(1, &buf->VBO);
    glGenBuffers(1, &buf->EBO);

    // 1. bind the VAO first - every buffer bind / attribute call below gets
    //    "recorded" into this VAO until we unbind it in step 5
    glBindVertexArray(buf->VAO);

    // 2. copy vertex positions into the VBO's GPU memory
    glBindBuffer(GL_ARRAY_BUFFER, buf->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, usage);

    // 3. copy index data into the EBO's GPU memory
    //    (this bind is what gets stored inside the currently-bound VAO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, usage);

    // 4. tell attribute slot 0 (aPos in the vertex shader) how to read the VBO:
    //    3 floats per vertex, not normalized, tightly packed, starting at offset 0
    // 1. Position (layout 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2. Disable the color array (layout 1) and force it to white so textures don't render black
    glDisableVertexAttribArray(1);
    glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);

    // 3. Texture (layout 2 in your current shader)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // glVertexAttribPointer already recorded the VBO into this attribute slot,
    // so it's safe to unbind GL_ARRAY_BUFFER now - the VAO remembers it either way
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // do NOT unbind GL_ELEMENT_ARRAY_BUFFER here - that binding lives inside
    // the VAO itself, so unbinding it now would remove the EBO from this VAO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 5. unbind the VAO so other glBindVertexArray/attribute calls elsewhere
    //    in the program can't accidentally change this VAO's setup
    glBindVertexArray(0);

    return buf;
}
