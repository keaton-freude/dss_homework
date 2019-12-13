#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, TexCoord);
    FragColor[3] = 1.0f;
} 