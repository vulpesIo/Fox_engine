#include <math.h>
#include "camera.h"
#include "cglm/vec3.h"

float YAW = -90.0f;
float PITCH = 0.0f;
float SPEED = 2.5f;
float SENSITIVITY = 0.1f;
float ZOOM = 45.0f;

vec3 Position = {0.0f, 0.0f, 3.0f};
vec3 Front = {0.0f, 0.0f, -1.0f};
vec3 Up = {0.0f, 1.0f, 0.0f};
vec3 Right = {1.0f, 0.0f, 0.0f};
vec3 WorldUp = {0.0f, 1.0f, 0.0f};

float Yaw = -90.0f;
float Pitch = 0.0f;
float MovementSpeed = 2.5f;
float MouseSensitivity = 0.1f;
float Zoom = 45.0f;

void create_camera(vec3 position, vec3 up, float yaw, float pitch) {
    glm_vec3_copy(position, Position);
    glm_vec3_copy(up, WorldUp);

    Yaw = yaw;
    Pitch = pitch;

    _updateCameraVectors();
}

void GetViewMatrix(mat4 matrix) {
    vec3 target;

    glm_vec3_add(Position, Front, target);
    glm_lookat(Position, target, Up, matrix);
}

void ProcessKeyBoard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        glm_vec3_muladds(Front, velocity, Position);
    if (direction == BACKWARD)
        glm_vec3_muladds(Front, -velocity, Position);
    if (direction == RIGHT)
        glm_vec3_muladds(Right, -velocity, Position);
    if (direction == LEFT)
        glm_vec3_muladds(Right, velocity, Position);
}

void ProcessMouseMovement(float xoffset, float yoffset, int constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;

        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    _updateCameraVectors();
}


void ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void _updateCameraVectors() {
    vec3 front;

    front[0] = cos(glm_rad(Yaw)) * cos(glm_rad(Pitch));
    front[1] = sin(glm_rad(Pitch));
    front[2] = sin(glm_rad(Yaw)) * cos(glm_rad(Pitch));

    // Normalize and update Front
    glm_vec3_normalize(front);
    glm_vec3_copy(front, Front);

    // Recalculate Right and Up vectors
    glm_vec3_cross(Front, WorldUp, Right);
    glm_vec3_normalize(Right);

    glm_vec3_cross(Right, Front, Up);
    glm_vec3_normalize(Up);
}
