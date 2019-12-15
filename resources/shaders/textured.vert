#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Up until this point all positions are within a "unit" rectangle
// Use the screen dimensions 
uniform uvec2 ScreenDimensions;

void main()
{
    TexCoord = aTexCoord;
    // Modify the Model to scale up the translation portion based on ScreenWidth
    mat4 MVP = Projection * View * Model;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}