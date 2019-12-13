#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertColor;

uniform mat4 MVP;

void main()
{
    vertColor = aColor;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}