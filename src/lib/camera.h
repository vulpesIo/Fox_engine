#ifndef CAMERA
#define CAMERA

#include "cglm/cglm.h"
#include "cglm/types.h"

typedef enum {
    FORWARD, BACKWARD,
    LEFT, RIGHT
} Camera_Movement;

extern float YAW;
extern float PITCH;
extern float SPEED;
extern float SENSITIVITY;
extern float ZOOM;

// camera Attributes
extern vec3 Position;
extern vec3 Front;
extern vec3 Up;
extern vec3 Right;
extern vec3 WorldUp;

// euler Angles
extern float Yaw;
extern float Pitch;

// camera options
extern float MovementSpeed;
extern float MouseSensitivity;
extern float Zoom;

void create_camera(vec3 position, vec3 up, float yaw, float pitch);

void GetViewMatrix(mat4 matrix);

void ProcessKeyBoard(Camera_Movement direction, float deltaTime);

void ProcessMouseMovement(float xoffset, float yoffset, int constrainPitch);

void ProcessMouseScroll(float yoffset);

void _updateCameraVectors();

#endif
