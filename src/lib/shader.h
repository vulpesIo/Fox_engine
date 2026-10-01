#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

typedef struct {
    unsigned int ID;
} Shader;

Shader shader_create(const char *vertexPath, const char *fragmentPath);

void shader_use(Shader *shader);

void shader_set_bool(Shader *shader, char *name, int value);
void shader_set_int(Shader *shader, char *name, int value);
void shader_set_float(Shader *shader, char *name, float value);

#endif