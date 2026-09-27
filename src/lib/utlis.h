#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <glad/glad.h>


typedef struct a {
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
} Buffer;

double get_elapsed_seconds(struct timespec start, struct timespec end);

Buffer* create_mesh_buffers(float vertices[], size_t vertices_size, int indices[], size_t indices_size, GLenum usage);

#endif