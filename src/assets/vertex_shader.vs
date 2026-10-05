#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float xOffset;
uniform float yOffset;
uniform float scaleF = 1.0;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    //vec4((aPos.x + xOffset) * scaleF, (-aPos.y + yOffset) * scaleF , (aPos.z) * scaleF, 1.0); // just add a - to the y position
    ourColor = aColor;
    TexCoord = aTexCoord;
}