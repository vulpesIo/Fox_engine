#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct {
    unsigned int ID;
} Shader;

Shader shader_create(const char *vertexPath, const char *fragmentPath);

void shader_use(Shader *shader);

void shader_set_bool(Shader *shader, char *name, int value);
void shader_set_int(Shader *shader, char *name, int value);
void shader_set_float(Shader *shader, char *name, float value);

void shader_set_mat2(Shader *shader, char *name, mat2 value);
void shader_set_mat3(Shader *shader, char *name, mat3 value);
void shader_set_mat4(Shader *shader, char *name, mat4 value);

#endif