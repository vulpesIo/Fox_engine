#ifndef CAMERA
#define CAMERA

#include "cglm/cglm.h"";
#include "cglm/types.h"

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// camera Attributes
vec3 Position;
vec3 Front;
vec3 Up;
vec3 Right;
vec3 WorldUp;

// euler Angles
float Yaw;
float Pitch;
// camera options
float MovementSpeed;
float MouseSensitivity;
float Zoom;

#endif
