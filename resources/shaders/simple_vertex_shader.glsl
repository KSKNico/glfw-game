#version 330
uniform mat4 MVP;
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
out vec3 Color;
void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    Color = vCol;
};