// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;

// out vec3 ourColor;

// void main()
// {
//     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//     ourColor = aColor;
// }

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float xOffset;
uniform float yOffset;
uniform float scaleF = 1.0;

void main()
{
    gl_Position = vec4((aPos.x + xOffset) * scaleF, (-aPos.y + yOffset) * scaleF , (aPos.z) * scaleF, 1.0); // just add a - to the y position
    ourColor = aColor;
}

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;

// // out vec3 ourColor;
// out vec3 ourPosition;

// void main()
// {
//     gl_Position = vec4(aPos, 1.0); 
//     // ourColor = aColor;
//     ourPosition = aPos;
// }