#include "shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

long get_file_size(const char *filename) {
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {
        fprintf(stderr, "ERROR::FILE_NOT_FOUND_OR_UNREADABLE: '%s'\n", filename);
        perror("Details");
        fflush(stderr);
        exit(1);
    }
    
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fprintf(stderr, "ERROR::FILE_SEEK_FAILED: '%s'\n", filename);
        fflush(stderr);
        fclose(fp);
        exit(1);
    }

    long size = ftell(fp);
    if (size < 0) {
        fprintf(stderr, "ERROR::FILE_TELL_FAILED: '%s'\n", filename);
        fflush(stderr);
        fclose(fp);
        exit(1);
    }
    
    fclose(fp);
    return size;
}

Shader shader_create(const char *vertexPath, const char *fragmentPath) {
    long vertexFileSize = get_file_size(vertexPath);
    long fragmentFileSize = get_file_size(fragmentPath);
    
    char* vertexFileContentBuffer = malloc((size_t)vertexFileSize + 1);
    if (vertexFileContentBuffer == NULL) {
        fprintf(stderr, "ERROR::MEMORY_ALLOCATION_FAILED for vertex shader buffer (%ld bytes)\n", vertexFileSize + 1);
        fflush(stderr);
        exit(1);
    }
    
    FILE *fp = fopen(vertexPath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "ERROR::FAILED_TO_OPEN_VERTEX_SHADER: '%s'\n", vertexPath);
        perror("Details");
        fflush(stderr);
        free(vertexFileContentBuffer);
        exit(1);
    }

    size_t bytes_read = fread(vertexFileContentBuffer, 1, (size_t)vertexFileSize, fp);
    if (bytes_read < (size_t)vertexFileSize) {
        fprintf(stderr, "ERROR::VERTEX_FILE_READ_INCOMPLETE: Expected %ld bytes, read %zu bytes from '%s'\n",
                vertexFileSize, bytes_read, vertexPath);
        if (ferror(fp)) {
            perror("Details");
        }
        fflush(stderr);
        free(vertexFileContentBuffer);
        fclose(fp);
        exit(1);
    }
    vertexFileContentBuffer[bytes_read] = '\0';
    fclose(fp);
    
    char* fragmentFileContentBuffer = malloc((size_t)fragmentFileSize + 1);
    if (fragmentFileContentBuffer == NULL) {
        fprintf(stderr, "ERROR::MEMORY_ALLOCATION_FAILED for fragment shader buffer (%ld bytes)\n", fragmentFileSize + 1);
        fflush(stderr);
        free(vertexFileContentBuffer);
        exit(1);
    }
    
    fp = fopen(fragmentPath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "ERROR::FAILED_TO_OPEN_FRAGMENT_SHADER: '%s'\n", fragmentPath);
        perror("Details");
        fflush(stderr);
        free(fragmentFileContentBuffer);
        exit(1);
    }

    bytes_read = fread(fragmentFileContentBuffer, 1, (size_t)fragmentFileSize, fp);
    if (bytes_read < (size_t)fragmentFileSize) {
        fprintf(stderr, "ERROR::FRAGMENT_FILE_READ_INCOMPLETE: Expected %ld bytes, read %zu bytes from '%s'\n",
                fragmentFileSize, bytes_read, fragmentPath);
        if (ferror(fp)) {
            perror("Details");
        }
        fflush(stderr);
        free(fragmentFileContentBuffer);
        fclose(fp);
        exit(1);
    }
    fragmentFileContentBuffer[bytes_read] = '\0';
    fclose(fp);
    
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char*[]){ vertexFileContentBuffer }, NULL);
    glCompileShader(vertex);
    free(vertexFileContentBuffer);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        fflush(stderr);
        free(fragmentFileContentBuffer);
        exit(1);
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char*[]){ fragmentFileContentBuffer }, NULL);
    glCompileShader(fragment);
    free(fragmentFileContentBuffer);
    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
        fflush(stderr);
        exit(1);
    }

    Shader shader;
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);

    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        fflush(stderr);
        exit(1);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

void shader_use(Shader *shader) {
    glUseProgram(shader->ID);
}

void shader_set_bool(Shader *shader,char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), (int)value); 
}


void shader_set_int(Shader *shader,char* name, int value)
{ 
    glUniform1i(glGetUniformLocation(shader->ID, name), value); 
}
void shader_set_float(Shader *shader,char* name, float value)
{ 
    glUniform1f(glGetUniformLocation(shader->ID, name), value); 
}