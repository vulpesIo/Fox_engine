#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// uniform vec4 ourColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}

// #version 330 core
// out vec4 FragColor;
// // in vec3 ourColor;
// in vec3 ourPosition;

// void main()
// {
//     FragColor = vec4(ourPosition, 1.0);    // note how the position value is linearly interpolated to get all the different colors
// }