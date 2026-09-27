#include "utlis.h"
#include <glad/glad.h>
#include <stdlib.h>

double get_elapsed_seconds(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec 
                   + ((double)start.tv_nsec / 1000000000.0);

    double end_sec = (double)end.tv_sec
                   + ((double)end.tv_nsec / 1000000000.0);
                   
    return end_sec - start_sec;
}

Buffer* create_mesh_buffers(float vertices[], size_t vertices_size, int indices[], size_t indices_size, GLenum usage) {
    Buffer* buf = malloc(sizeof(Buffer));
    
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
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