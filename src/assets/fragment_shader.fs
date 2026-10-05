// #version 330 core
// out vec4 FragColor;

// in vec3 ourColor;
// in vec2 TexCoord;

// // uniform vec4 ourColor;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

// void main()
// {
//     FragColor =  texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
// }

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // Linearly blend container (80%) and smiley face (20%)
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), 0.2);
}

// #version 330 core
// out vec4 FragColor;
// // in vec3 ourColor;
// in vec3 ourPosition;

// void main()
// {
//     FragColor = vec4(ourPosition, 1.0);    // note how the position value is linearly interpolated to get all the different colors
// }