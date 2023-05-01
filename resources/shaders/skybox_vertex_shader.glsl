#version 330

uniform mat4 MVP;

layout (location = 0) in vec3 aPos;

out vec3 TextureVector;

void main()
{
    vec4 pos = MVP * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    TextureVector = aPos;
};